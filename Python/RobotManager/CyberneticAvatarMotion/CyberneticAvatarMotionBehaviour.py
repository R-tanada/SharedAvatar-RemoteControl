# ----------------------------------------------------------------------- 
# Author:   Takayoshi Hagiwara (KMD)
# Created:  2021/8/20
# Summary:  Cybernetic avatar の運動制御マネージャー
# -----------------------------------------------------------------------

import numpy as np
import scipy.spatial.transform as scitransform
import quaternion
import math

"""
##### IMPORTANT #####
If you are using average rotations of three or more people, please cite the following paper
see also: https://github.com/UCL/scikit-surgerycore

Thompson S, Dowrick T, Ahmad M, et al.
SciKit-Surgery: compact libraries for surgical navigation.
International Journal of Computer Assisted Radiology and Surgery. May 2020.
DOI: 10.1007/s11548-020-02180-5
"""
import sksurgerycore.algorithms.averagequaternions as aveq


class CyberneticAvatarMotionBehaviour:

    originPositions     = {}
    inversedMatrixforPosition ={}
    inversedMatrix      = {}

    beforePositions     = {}
    weightedPositions   = {}

    beforeRotations     = {}
    weightedRotations   = {}

    def __init__(self, defaultParticipantNum: int = 2) -> None:
        for i in range(defaultParticipantNum):
            self.originPositions['participant'+str(i+1)] = np.zeros(3)
            self.inversedMatrixforPosition['participant'+str(i+1)] = np.array([[1,0,0],[0,1,0],[0,0,1]])
            self.inversedMatrix['participant'+str(i+1)] = np.array([[1,0,0,0],[0,1,0,0],[0,0,1,0],[0,0,0,1]])
            
            self.beforePositions['participant'+str(i+1)] = np.zeros(3)
            self.weightedPositions['participant'+str(i+1)] = np.zeros(3)

            self.beforeRotations['participant'+str(i+1)] = np.array([0,0,0,1])
            self.weightedRotations['participant'+str(i+1)] = np.array([0,0,0,1])
        
        self.participantNum = defaultParticipantNum
        self.before_position = [[0, 0, 0],[0, 0, 0]]
        self.customweightPosition = [0, 0, 0]
        self.before_sharedPosition = [0, 0, 0]


    def GetSharedTransform(self, position: dict, rotation: dict, method: str = 'integration', weight: float = 0.5, isRelativePosition: bool = True, isRelativeRotation: bool = True):
        """
        Calculate the shared transforms.

        Parameters
        ----------
        position: dict or numpy array
            Participants' rigid body position. 
            [x, y, z]
        rotation: dict or numpy array
            Participants' rigid body rotation.
            [x, y, z, w]
        method: (Optional) str
            Shared methods.
            Options: 'integration', 'divisionofroles', 'customweight'
        weight: (Optional) float
            Shared weight.
            0 = 100% Participant2
            1 = 100% Participant1
            If there are more than three participants, use the average value.
        isRelativePosition: (Optional) bool
            Use relative position
        isRelativeRotation: (Optional) bool
            Use relative rotation
        
        Returns
        ----------
        sharedPosition: dict
            Shared position.
            [x, y, z]
        sharedRotation: numpy array
            Shared rotation as Euler angles.
            [x, y, z]
        """

        # ----- numpy array to dict: position ----- #
        if type(position) is np.ndarray:
            position = self.NumpyArray2Dict(position)
        
        # ----- numpy array to dict: rotation ----- #
        if type(rotation) is np.ndarray:
            rotation = self.NumpyArray2Dict(rotation)


        if isRelativePosition:
            pos = self.GetRelativePosition(position)
        else:
            pos = position
        
        if isRelativeRotation:
            rot = self.GetRelativeRotation(rotation)
        else:
            rot = rotation
        
        sharedMethods = method.lower()
        if sharedMethods == 'integration':
            sharedPosition, sharedRotation = self.IntegratedMotion(pos, rot, weight)
        elif sharedMethods == 'divisionofroles':
            sharedPosition, sharedRotation = self.DivisionOfRolesMotion(pos, rot)
        elif sharedMethods == 'customweight':
            sharedPosition, sharedRotation = self.CustomWeight(pos, rot, weight)
        # print("sharedPosition",sharedPosition)
        
        return sharedPosition, sharedRotation
    
    def GetSharedTransformWithCustomWeight(self, position: dict, rotation: dict, weight: list, isRelativePosition: bool = True, isRelativeRotation: bool = True):
        """
        Calculate the shared Transform with custom weights for each participant.
        
        ----- CAUTION -----
            Since the angle is converted to Euler angle during the calculation, the exact angle may not be provided.
            In addition, the behavior near the singularity is unstable.

        Parameters
        ----------
        position: dict or numpy array
            Participants' rigid body position. 
            [x, y, z]
        rotation: dict or numpy array
            Participants' rigid body rotation as Quaternion.
            [x, y, z, w]
        weight: list
            Weights for each participant.
            A list corresponding to the number of participants
            
            ----- If the dimension of the list is 1 -----
            len(weight) = Number of participants

            Example: Number of participant = 2
                -> [0.5, 0.5]
            
            ----- If the dimension of the list is 2 -----
            For weight slider.
            weight[0][0]: Participant1 pos, weight[0][1]: Participant2 pos
            weight[1][0]: Participant1 rot, weight[1][1]: Participant2 rot
        isRelativePosition: (Optional) bool
            Use relative position
        isRelativeRotation: (Optional) bool
            Use relative rotation
        
        Returns
        ----------
        sharedPosition: numpy array
            Shared position.
            [x, y, z]
        sharedRotation_euler: numpy array
            Shared rotation as Euler angles.
            [x, y, z]
        """

        # ----- numpy array to dict: position ----- #
        if type(position) is np.ndarray:
            position = self.NumpyArray2Dict(position)
        
        # ----- numpy array to dict: rotation ----- #
        if type(rotation) is np.ndarray:
            rotation = self.NumpyArray2Dict(rotation)
        
        if isRelativePosition:
            pos = self.GetRelativePosition(position)
        else:
            pos = position
        
        if isRelativeRotation:
            rot = self.GetRelativeRotation(rotation)
        else:
            rot = rotation

        # ----- Shared transform ----- #
        sharedPosition = [0, 0, 0]
        sharedRotation_euler = [0, 0, 0]

        weightListDim = np.array(weight).ndim

        if weightListDim == 1:
            for i in range(self.participantNum):
                sharedPosition += pos['participant'+str(i+1)] * weight[i]
                sharedRotation_euler += self.Quaternion2Euler(rot['participant'+str(i+1)]) * weight[i]
        elif weightListDim == 2:
            for i in range(self.participantNum):
                # ----- Position ----- #
                diffPos     = pos['participant'+str(i+1)] - self.beforePositions['participant'+str(i+1)]
                weightedPos = diffPos * weight[0][i] + self.weightedPositions['participant'+str(i+1)]
                sharedPosition += weightedPos

                self.weightedPositions['participant'+str(i+1)]  = weightedPos
                self.beforePositions['participant'+str(i+1)]    = pos['participant'+str(i+1)]

                # ----- Rotation ----- #
                qw, qx, qy, qz = self.beforeRotations['participant'+str(i+1)][3], self.beforeRotations['participant'+str(i+1)][0], self.beforeRotations['participant'+str(i+1)][1], self.beforeRotations['participant'+str(i+1)][2]
                mat4x4 = np.array([ [qw, qz, -qy, qx],
                                    [-qz, qw, qx, qy],
                                    [qy, -qx, qw, qz],
                                    [-qx,-qy, -qz, qw]])
                currentRot = rot['participant'+str(i+1)]
                diffRot = np.dot(np.linalg.inv(mat4x4), currentRot)
                diffRotEuler = self.Quaternion2Euler(np.array(diffRot))
                
                weightedDiffRotEuler = list(map(lambda x: x * weight[1][i] , diffRotEuler))
                weightedDiffRot = self.Euler2Quaternion(np.array(weightedDiffRotEuler))

                nqw, nqx, nqy, nqz = weightedDiffRot[3], weightedDiffRot[0], weightedDiffRot[1], weightedDiffRot[2]
                neomat4x4 = np.array([[nqw, -nqz, nqy, nqx],
                                     [nqz, nqw, -nqx, nqy],
                                     [-nqy, nqx, nqw, nqz],
                                     [-nqx,-nqy, -nqz, nqw]])
                weightedRot = np.dot(neomat4x4,  self.weightedRotations['participant'+str(i+1)])
                sharedRotation_euler += self.Quaternion2Euler(weightedRot)

                self.weightedRotations['participant'+str(i+1)]  = weightedRot
                self.beforeRotations['participant'+str(i+1)]    = rot['participant'+str(i+1)]


                # ----- From Ogura-kun ----- #
                #diff_Position = list(map(lambda x, y: x - y, list(pos['participant'+str(i+1)]), self.before_position[i]))
                #a = list(map(lambda x: x * weight[0][i] , diff_Position))
                #self.customweightPosition = list(map(lambda x,y: x + y, a, self.customweightPosition))
                #self.before_position[i] = list(pos['participant'+str(i+1)])
                #customweightRotation_euler += self.Quaternion2Euler(rot['participant'+str(i+1)]) * weight[1][i]

                # ----- REGACY: Hagi system ----- #
                #sharedPosition += pos['participant'+str(i+1)] * weight[0][i]
                #sharedRotation_euler += self.Quaternion2Euler(rot['participant'+str(i+1)]) * weight[1][i]

        return sharedPosition, sharedRotation_euler


    def DualArmTransform(self, position: dict, rotation: dict, weight: list, isRelativePosition: bool = True, isRelativeRotation: bool = True):
        """
        Calculate the shared Transform with custom weights for each participant.
        
        ----- CAUTION -----
            Since the angle is converted to Euler angle during the calculation, the exact angle may not be provided.
            In addition, the behavior near the singularity is unstable.

        Parameters
        ----------
        position: dict or numpy array
            Participants' rigid body position. 
            [x, y, z]
        rotation: dict or numpy array
            Participants' rigid body rotation as Quaternion.
            [x, y, z, w]
        weight: list
            Weights for each participant.
            A list corresponding to the number of participants
            
            ----- If the dimension of the list is 1 -----
            len(weight) = Number of participants

            Example: Number of participant = 2
                -> [0.5, 0.5]
            
            ----- If the dimension of the list is 2 -----
            For weight slider.
            weight[0][0]: Participant1 pos, weight[0][1]: Participant2 pos
            weight[1][0]: Participant1 rot, weight[1][1]: Participant2 rot
        isRelativePosition: (Optional) bool
            Use relative position
        isRelativeRotation: (Optional) bool
            Use relative rotation
        
        Returns
        ----------
        sharedPosition: numpy array
            Shared position.
            [x, y, z]
        sharedRotation_euler: numpy array
            Shared rotation as Euler angles.
            [x, y, z]
        """

        # ----- numpy array to dict: position ----- #
        if type(position) is np.ndarray:
            position = self.NumpyArray2Dict(position)
        
        # ----- numpy array to dict: rotation ----- #
        if type(rotation) is np.ndarray:
            rotation = self.NumpyArray2Dict(rotation)
        
        if isRelativePosition:
            pos = self.GetRelativePosition(position)
        else:
            pos = position
        
        if isRelativeRotation:
            rot = self.GetRelativeRotation(rotation)
        else:
            rot = rotation

        # ----- Shared transform ----- #
        sharedPosition = [0, 0, 0]
        sharedRotation_euler = [0, 0, 0]

        weightListDim = np.array(weight).ndim

        if weightListDim == 1:
            for i in range(self.participantNum):
                sharedPosition += pos['participant'+str(i+1)] * weight[i]
                sharedRotation_euler += self.Quaternion2Euler(rot['participant'+str(i+1)]) * weight[i]
        elif weightListDim == 2:
            for i in range(self.participantNum):
                # ----- Position ----- #
                diffPos     = pos['participant'+str(i+1)] - self.beforePositions['participant'+str(i+1)]
                weightedPos = diffPos * weight[0][i] + self.weightedPositions['participant'+str(i+1)]
                # sharedPosition += weightedPos

                self.weightedPositions['participant'+str(i+1)]  = weightedPos
                self.beforePositions['participant'+str(i+1)]    = pos['participant'+str(i+1)]

                # ----- Rotation ----- #
                qw, qx, qy, qz = self.beforeRotations['participant'+str(i+1)][3], self.beforeRotations['participant'+str(i+1)][0], self.beforeRotations['participant'+str(i+1)][1], self.beforeRotations['participant'+str(i+1)][2]
                mat4x4 = np.array([ [qw, qz, -qy, qx],
                                    [-qz, qw, qx, qy],
                                    [qy, -qx, qw, qz],
                                    [-qx,-qy, -qz, qw]])
                currentRot = rot['participant'+str(i+1)]
                diffRot = np.dot(np.linalg.inv(mat4x4), currentRot)
                diffRotEuler = self.Quaternion2Euler(np.array(diffRot))
                
                weightedDiffRotEuler = list(map(lambda x: x * weight[1][i] , diffRotEuler))
                weightedDiffRot = self.Euler2Quaternion(np.array(weightedDiffRotEuler))

                nqw, nqx, nqy, nqz = weightedDiffRot[3], weightedDiffRot[0], weightedDiffRot[1], weightedDiffRot[2]
                neomat4x4 = np.array([[nqw, -nqz, nqy, nqx],
                                     [nqz, nqw, -nqx, nqy],
                                     [-nqy, nqx, nqw, nqz],
                                     [-nqx,-nqy, -nqz, nqw]])
                weightedRot = np.dot(neomat4x4,  self.weightedRotations['participant'+str(i+1)])
                # sharedRotation_euler = self.Quaternion2Euler(weightedRot)

                self.weightedRotations['participant'+str(i+1)]  = weightedRot
                self.beforeRotations['participant'+str(i+1)]    = rot['participant'+str(i+1)]


                # ----- From Ogura-kun ----- #
                #diff_Position = list(map(lambda x, y: x - y, list(pos['participant'+str(i+1)]), self.before_position[i]))
                #a = list(map(lambda x: x * weight[0][i] , diff_Position))
                #self.customweightPosition = list(map(lambda x,y: x + y, a, self.customweightPosition))
                #self.before_position[i] = list(pos['participant'+str(i+1)])
                #customweightRotation_euler += self.Quaternion2Euler(rot['participant'+str(i+1)]) * weight[1][i]

                # ----- REGACY: Hagi system ----- #
                #sharedPosition += pos['participant'+str(i+1)] * weight[0][i]
                #sharedRotation_euler += self.Quaternion2Euler(rot['participant'+str(i+1)]) * weight[1][i]

        return self.weightedPositions['participant1'],self.weightedRotations['participant1'],self.weightedPositions['participant2'],self.weightedRotations['participant2']
    

    def IntegratedMotion(self, pos, rot, weight):
        """
        Calculate the weighted integration transforms.

        Parameters
        ----------
        pos: dict
            Participants' rigid body position. 
            [x, y, z]
        rot: dict
            Participants' rigid body rotation.
            [x, y, z, w]
        weight: float
            Integrated weight.
            0 = 100% Participant2
            1 = 100% Participant1
            If there are more than three participants, use the average value.
        
        Returns
        ----------
        integratedPosition: dict
            Integrated position.
            [x, y, z]
        integratedRotation_euler: numpy array
            Integrated rotation as Euler angles.
            [x, y, z]
        """

        # ----- Position ----- #
        if self.participantNum < 2:
            integratedPosition = pos['participant1']
        elif self.participantNum == 2:
            integratedPosition = pos['participant1'] * weight + pos['participant2'] * (1 - weight)
        else:
            # If there are more than three, use the average value.
            npPosition = np.array(list(pos.values()))
            integratedPosition = npPosition.mean(axis=0)

        # ----- Rotation ----- #
        if self.participantNum < 2:
            integratedRotation_euler = self.Quaternion2Euler(rot['participant1'])
        elif self.participantNum == 2:
            rot1 = rot['participant1']
            q1 = np.quaternion(rot1[3], rot1[0], rot1[1], rot1[2])

            rot2 = rot['participant2']
            q2 = np.quaternion(rot2[3], rot2[0], rot2[1], rot2[2])

            slerpRotation   = quaternion.slerp_evaluate(q1, q2, (1-weight))
            npSlerpRotation = quaternion.as_float_array(slerpRotation)
            integratedRotation_euler  = self.Quaternion2Euler(np.array([npSlerpRotation[1], npSlerpRotation[2], npSlerpRotation[3], npSlerpRotation[0]]))
        else:
            # If there are more than three, use the average value.
            # Also, If you are using average rotations of three or more people, please cite the above papers
            npRotation = np.array(list(rot.values()))
            integratedRotation_euler = self.Quaternion2Euler(aveq.average_quaternions(npRotation))
        
        return integratedPosition, integratedRotation_euler


    def DivisionOfRolesMotion(self, pos, rot, roles: list = ['participant1', 'participant2']):
        """
        Calculate the division of roles transforms.

        Parameters
        ----------
        pos: dict
            Participants' rigid body position. 
            [x, y, z]
        rot: dict
            Participants' rigid body rotation.
            [x, y, z, w]
        roles: (Optional) list(str)
            Division of roles for position and posture
            roles[0]: Position
            roles[1]: Posture
        Returns
        ----------
        divisionOfRolesPosition: dict
            Division of roles position.
            [x, y, z]
        divisionOfRolesRotation_euler: numpy array
            Division of roles rotation as Euler angles.
            [x, y, z]
        """

        if self.participantNum < 2:
            divisionOfRolesPosition = pos['participant1']
            divisionOfRolesRotation_euler = self.Quaternion2Euler(rot['participant1'])

        elif self.participantNum == 2:
            divisionOfRolesPosition = pos[roles[0]]
            divisionOfRolesRotation_euler = self.Quaternion2Euler(rot[roles[1]])

        return divisionOfRolesPosition, divisionOfRolesRotation_euler


    def CustomWeight(self, pos, rot, weight):
        """
        Calculate the custom-weighted integration transforms.

        Parameters
        ----------
        pos: dict
            Participants' rigid body position. 
            [x, y, z]
        rot: dict
            Participants' rigid body rotation.
            [x, y, z, w]
       weight: list
            Weights for each participant.
            A list corresponding to the number of participants
            
            ----- If the dimension of the list is 1 -----
            len(weight) = Number of participants

            Example: Number of participant = 2
                -> [0.5, 0.5]
            
            ----- If the dimension of the list is 2 -----
            For weight slider.
            weight[0][0]: Participant1 pos, weight[0][1]: Participant2 pos
            weight[1][0]: Participant1 rot, weight[1][1]: Participant2 rot
        
        Returns
        ----------
        customweightPosition: numpy array
            Custom-weighted position.
            [x, y, z]
        customweightRotation_euler: numpy array
            Integrated rotation as Euler angles.
            [x, y, z]
        """

        # ----- Shared transform ----- #
        customweightRotation_euler = [0, 0, 0]

        weightListDim = np.array(weight).ndim

        if weightListDim == 1:
            for i in range(self.participantNum):
                self.customweightPosition += pos['participant'+str(i+1)] * weight[i]
                customweightRotation_euler += self.Quaternion2Euler(rot['participant'+str(i+1)]) * weight[i]
        elif weightListDim == 2:
            for i in range(self.participantNum):
                diff_Position = list(map(lambda x, y: x - y, list(pos['participant'+str(i+1)]), self.before_position[i]))
                a = list(map(lambda x: x * weight[0][i] , diff_Position))
                self.customweightPosition = list(map(lambda x,y: x + y, a, self.customweightPosition))
                self.before_position[i] = list(pos['participant'+str(i+1)])
                customweightRotation_euler += self.Quaternion2Euler(rot['participant'+str(i+1)]) * weight[1][i]
                
        return np.array(self.customweightPosition), customweightRotation_euler

    def SetOriginPosition(self, position) -> None:
        """
        Set the origin position

        Parameters
        ----------
        position: dict, numpy array
            Origin position
        """
        # ----- numpy array to dict: position ----- #
        if type(position) is np.ndarray:
            position = self.NumpyArray2Dict(position)
        
        #print(position)

        listParticipant = [participant for participant in list(position.keys()) if 'participant' in participant]
        self.participantNum = len(listParticipant)
        
        for i in range(self.participantNum):
            self.originPositions['participant'+str(i+1)] = position['participant'+str(i+1)]
        
    def GetRelativePosition(self, position):
        """
        Get the relative position

        Parameters
        ----------
        position: dict, numpy array
            Position to compare with the origin position.
            [x, y, z]
        
        Returns
        ----------
        relativePos: dict
            Position relative to the origin position.
            [x, y, z]
        """

        # ----- numpy array to dict: position ----- #
        if type(position) is np.ndarray:
            position = self.NumpyArray2Dict(position)
        
        relativePos = {}
        for i in range(self.participantNum):
            relativePos['participant'+str(i+1)] = position['participant'+str(i+1)] - self.originPositions['participant'+str(i+1)]
        
        return relativePos
    

    def SetInversedMatrix(self, rotation) -> None:
        """
        Set the inversed matrix

        Parameters
        ----------
        rotation: dict, numpy array
            Quaternion.
            Rotation for inverse matrix calculation
        """

        # ----- numpy array to dict: rotation ----- #
        if type(rotation) is np.ndarray:
            rotation = self.NumpyArray2Dict(rotation)
        
        listParticipant = [participant for participant in list(rotation.keys()) if 'participant' in participant]
        self.participantNum = len(listParticipant)

        for i in range(self.participantNum):
            q = rotation['participant'+str(i+1)]
            qw, qx, qy, qz = q[3], q[1], q[2], q[0]
            mat4x4 = np.array([ [qw, -qy, qx, qz],
                                [qy, qw, -qz, qx],
                                [-qx, qz, qw, qy],
                                [-qz,-qx, -qy, qw]])
            self.inversedMatrix['participant'+str(i+1)] = np.linalg.inv(mat4x4)

    def GetRelativePosition_r(self, position):
        """
        Get the relative position
        Parameters
        ----------
        position: dict, numpy array
            Position to compare with the origin position.
            [x, y, z]
        
        Returns
        ----------
        relativePos: dict
            Position relative to the origin position.
            [x, y, z]
        """

        # ----- numpy array to dict: position ----- #
        if type(position) is np.ndarray:
            position = self.NumpyArray2Dict(position)
        
        relativePos = {}
        for i in range(self.participantNum):
            relativePos['participant'+str(i+1)] = np.dot(self.inversedMatrixforPosition['participant'+str(i+1)],position['participant'+str(i+1)] - self.originPositions['participant'+str(i+1)])
        relativePos['endEffector'] = np.dot(self.inversedMatrixforPosition['endEffector'],position['endEffector'] - self.originPositions['endEffector'])
        
        return relativePos
    
    def GetRelativeRotation(self, rotation):
        """
        Get the relative rotation

        Parameters
        ----------
        rotation: dict, numpy array
            Rotation to compare with the origin rotation.
            [x, y, z, w]
        
        Returns
        ----------
        relativeRot: dict
            Rotation relative to the origin rotation.
            [x, y, z, w]
        """

        # ----- numpy array to dict: rotation ----- #
        if type(rotation) is np.ndarray:
            rotation = self.NumpyArray2Dict(rotation)
        
        relativeRot = {}
        for i in range(self.participantNum):
            relativeRot['participant'+str(i+1)] = np.dot(self.inversedMatrix['participant'+str(i+1)], rotation['participant'+str(i+1)])

        return relativeRot

    def GetRelativeRotation_r(self, rotation):
        """
        Get the relative rotation

        Parameters
        ----------
        rotation: dict, numpy array
            Rotation to compare with the origin rotation.
            [x, y, z, w]
        
        Returns
        ----------
        relativeRot: dict
            Rotation relative to the origin rotation.
            [x, y, z, w]
        """

        # ----- numpy array to dict: rotation ----- #
        if type(rotation) is np.ndarray:
            rotation = self.NumpyArray2Dict(rotation)
        
        relativeRot = {}
        for i in range(self.participantNum):
            relativeRot['participant'+str(i+1)] = np.dot(self.inversedMatrix['participant'+str(i+1)], rotation['participant'+str(i+1)])
        relativeRot['endEffector'] = np.dot(self.inversedMatrix['endEffector'], rotation['endEffector'])

        return relativeRot

    def Quaternion2Euler(self, q, isDeg: bool = True):
        """
        Calculate the Euler angle from the Quaternion.

        
        Rotation matrix
        |m00 m01 m02 0|
        |m10 m11 m12 0|
        |m20 m21 m22 0|
        | 0   0   0  1|

        Parameters
        ----------
        q: np.ndarray
            Quaternion.
            [x, y, z, w]
        isDeg: (Optional) bool
            Returned angles are in degrees if this flag is True, else they are in radians.
            The default is True.
        
        Returns
        ----------
        rotEuler: np.ndarray
            Euler angle.
            [x, y, z]
        """
        
        qx = q[0]
        qy = q[1]
        qz = q[2]
        qw = q[3]

        # 1 - 2y^2 - 2z^2
        m00 = 1 - (2 * qy**2) - (2 * qz**2)
        # 2xy + 2wz
        m01 = (2 * qx * qy) + (2 * qw * qz)
        # 2xz - 2wy
        m02 = (2 * qx * qz) - (2 * qw * qy)
        # 2xy - 2wz
        m10 = (2 * qx * qy) - (2 * qw * qz)
        # 1 - 2x^2 - 2z^2
        m11 = 1 - (2 * qx**2) - (2 * qz**2)
        # 2yz + 2wx
        m12 = (2 * qy * qz) + (2 * qw * qx)
        # 2xz + 2wy
        m20 = (2 * qx * qz) + (2 * qw * qy)
        # 2yz - 2wx
        m21 = (2 * qy * qz) - (2 * qw * qx)
        # 1 - 2x^2 - 2y^2
        m22 = 1 - (2 * qx**2) - (2 * qy**2)

        # 回転軸の順番がX->Y->Zの固定角(Rz*Ry*Rx)
        # if m01 == -1:
        # 	tx = 0
        # 	ty = math.pi/2
        # 	tz = math.atan2(m20, m10)
        # elif m20 == 1:
        # 	tx = 0
        # 	ty = -math.pi/2
        # 	tz = math.atan2(m20, m10)
        # else:
        # 	tx = -math.atan2(m02, m00)
        # 	ty = -math.asin(-m01)
        # 	tz = -math.atan2(m21, m11)

        # 回転軸の順番がX->Y->Zのオイラー角(Rx*Ry*Rz)
        if m02 == 1:
            tx = math.atan2(m10, m11)
            ty = math.pi/2
            tz = 0
        elif m02 == -1:
            tx = math.atan2(m21, m20)
            ty = -math.pi/2
            tz = 0
        else:
            tx = -math.atan2(-m12, m22)
            ty = -math.asin(m02)
            tz = -math.atan2(-m01, m00)

        if isDeg:
            tx = np.rad2deg(tx)
            ty = np.rad2deg(ty)
            tz = np.rad2deg(tz)

        rotEuler = np.array([tx, ty, tz])
        return rotEuler
    
    def ScipyQuaternion2Euler(self, q, sequence: str = 'xyz', isDeg: bool = True):
        """
        Calculate the Euler angle from the Quaternion.
        Using scipy.spatial.transform.Rotation.as_euler

        Parameters
        ----------
        q: np.ndarray
            Quaternion.
            [x, y, z, w]
        sequence: (Optional) str
            Rotation sequence of Euler representation, specified as a string.
            The rotation sequence defines the order of rotations about the axes.
            The default is xyz.
        isDeg: (Optional) bool
            Returned angles are in degrees if this flag is True, else they are in radians.
            The default is True.
        
        Returns
        ----------
        rotEuler: np.ndarray
            Euler angle.
            [x, y, z]
        """

        quat = scitransform.Rotation.from_quat(q)
        rotEuler = quat.as_euler(sequence, degrees=isDeg)
        return rotEuler
    
    def Euler2Quaternion(self, e):
        """
        Calculate the Quaternion from the Euler angle.

        Parameters
        ----------
        e: np.ndarray
            Euler.
            [x, y, z]
        
        Returns
        ----------
        rotQuat: np.ndarray
            Quaternion
            [x, y, z, w]
        """

        roll = np.deg2rad(e[0])
        pitch = np.deg2rad(e[1])
        yaw = np.deg2rad(e[2])

        cosRoll = np.cos(roll/2.0)
        sinRoll = np.sin(roll / 2.0)
        cosPitch = np.cos(pitch / 2.0)
        sinPitch = np.sin(pitch / 2.0)
        cosYaw = np.cos(yaw / 2.0)
        sinYaw = np.sin(yaw / 2.0)

        q0 = cosRoll * cosPitch * cosYaw + sinRoll * sinPitch * sinYaw
        q1 = sinRoll * cosPitch * cosYaw - cosRoll * sinPitch * sinYaw
        q2 = cosRoll * sinPitch * cosYaw + sinRoll * cosPitch * sinYaw
        q3 = cosRoll * cosPitch * sinYaw - sinRoll * sinPitch * cosYaw

        rotQuat = [q1, q2, q3, q0]
        return rotQuat

    def ScipyEuler2Quaternion(self, e, sequence: str = 'xyz', isDeg: bool = True):
        """
        Calculate the Quaternion from the Euler angle.
        Using scipy.spatial.transform.Rotation.as_quat

        Parameters
        ----------
        e: np.ndarray
            Euler.
            [x, y, z]
        sequence: (Optional) str
            Rotation sequence of Euler representation, specified as a string.
            The rotation sequence defines the order of rotations about the axes.
            The default is xyz.
        isDeg: (Optional) bool
            If True, then the given angles are assumed to be in degrees. Default is True.
        
        Returns
        ----------
        rotQuat: np.ndarray
            Quaternion
            [x, y, z, w]
        """
        
        quat = scitransform.Rotation.from_euler(sequence, e, isDeg)
        rotQuat = quat.as_quat()
        return rotQuat
    
    def InversedRotation(self, rot, axes: list = []):
        """
        Calculate the inversed rotation.

        ----- CAUTION -----
        If "axes" is set, it will be converted to Euler angles during the calculation process, which may result in inaccurate rotation.
        In addition, the behavior near the singularity is unstable.

        Parameters
        ----------
        rot: np.ndarray
            Quaternion.
            [x, y, z, w]
        axes: (Optional) list[str]
            Axes to be inversed.
            If length of axes is zero, return inversed quaternion

        Returns
        ----------
        inversedRot: np.ndarray
            Inversed rotation
            [x, y, z, w]
        """

        if len(axes) == 0:
            quat = scitransform.Rotation.from_quat(rot)
            inversedRot = quat.inv().as_quat()
            return inversedRot

        rot = self.ScipyQuaternion2Euler(rot)

        for axis in axes:
            if axis == 'x':
                rot[0] = -rot[0]
            elif axis == 'y':
                rot[1] = -rot[1]
            elif axis == 'z':
                rot[2] = -rot[2]

        inversedRot = self.ScipyEuler2Quaternion(rot)

        return inversedRot

    def NumpyArray2Dict(self, numpyArray, dictKey: str = 'participant'):
        """
        Convert numpy array to dict.

        Parameters
        ----------
        numpyArray: numpy array
            Numpy array.
        dictKey: (Optional) str
            The key name of the dict.
        """
        
        if type(numpyArray) is np.ndarray:
            dictionary = {}
            if len(numpyArray.shape) == 1:
                dictionary[dictKey+str(1)] = numpyArray
            else:
                for i in range(len(numpyArray)):
                    dictionary[dictKey+str(i+1)] = numpyArray[i]
        else:
            print('Type Error: argument is NOT Numpy array')
            return
        
        return dictionary