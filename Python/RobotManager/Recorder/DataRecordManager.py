# -----------------------------------------------------------------------
# Author:   Takayoshi Hagiwara (KMD)
# Created:  2021/8/25
# Summary:  Data Recording Manager
# -----------------------------------------------------------------------

import os
import csv
import numpy as np
import tqdm
import datetime

class DataRecordManager:
    dictPosition = {}
    dictRotation = {}
    dictGripperValue = {}
    dictDurationTime = []

    def __init__(self, participantNum: int = 2, otherRigidBodyNum: int = 1, otherRigidBodyNames: list = ['endEffector'], bendingSensorNum: int = 1) -> None:
        """
        Initialize class: DataRecordManager

        Parameters
        ----------
        participantNum: (Optional) int
            Number of participants
        otherRigidBodyNum: (Optional) int
            Number of rigid body objects except participants' rigid body
        otherRigidBodyNames: (Optional) list(str)
            Name list of rigid body objects except participants' rigid body
        bendingSensorNum: (Optional) int
            Number of bending sensors
        """
        
        self.participantNum         = participantNum
        self.otherRigidBodyNum      = otherRigidBodyNum
        self.otherRigidBodyNames    = otherRigidBodyNames
        self.bendingSensorNum       = bendingSensorNum

        for i in range(self.participantNum):
            self.dictPosition['participant'+str(i+1)] = []
            self.dictRotation['participant'+str(i+1)] = []
        
        for i in range(self.otherRigidBodyNum):
            self.dictPosition[self.otherRigidBodyNames[i]] = []
            self.dictRotation[self.otherRigidBodyNames[i]] = []
        
        for i in range(self.bendingSensorNum):
            self.dictGripperValue['gripperValue'+str(i+1)] = []

    def Record(self, position, rotation, bendingSensor, duration):
        """
        Record the data.

        Parameters
        ----------
        position: dict
            Position
        rotation: dict
            Rotation
        bendingSensor: dict
            Bending sensor values
        """

        self.dictDurationTime.append(duration)

        for i in range(self.participantNum):
            self.dictPosition['participant'+str(i+1)].append(position['participant'+str(i+1)])
            self.dictRotation['participant'+str(i+1)].append(rotation['participant'+str(i+1)])
        
        for i in range(self.otherRigidBodyNum):
            self.dictPosition[self.otherRigidBodyNames[i]].append(position[self.otherRigidBodyNames[i]])
            self.dictRotation[self.otherRigidBodyNames[i]].append(rotation[self.otherRigidBodyNames[i]])
        
        for i in range(self.bendingSensorNum):
            self.dictGripperValue['gripperValue'+str(i+1)].append([bendingSensor['gripperValue'+str(i+1)]])
    

    def ExportSelf(self, dirPath: str = 'ExportData',participant: str = '',conditions:str = '',number:str = ''):
        """
        Export the data recorded in DataRecordManager as CSV format.

        Parameters
        ----------
        dirPath: (Optional) str
            Directory path (not include the file name).
        """
        transformHeader = ['time','x','y','z','qx','qy','qz','qw']
        bendingSensorHeader = ['bendingValue']

        print('\n---------- DataRecordManager.ExportSelf ----------')
        print('Writing: Participant transform...')
        for i in tqdm.tqdm(range(self.participantNum), ncols=150):
            npDuration = np.array(self.dictDurationTime)
            npPosition = np.array(self.dictPosition['participant'+str(i+1)])
            npRotation = np.array(self.dictRotation['participant'+str(i+1)])
            npParticipantTransform = np.concatenate([npPosition, npRotation], axis=1)
            npTimeParticipantTransform = np.c_[npDuration,npParticipantTransform]

            self.ExportAsCSV(npTimeParticipantTransform, dirPath, 'Transform_Participant_'+str(i+1), participant, conditions, number, transformHeader)
        
        print('Writing: Other rigid body transform...')
        for i in tqdm.tqdm(range(self.otherRigidBodyNum), ncols=150):
            npPosition = np.array(self.dictPosition[self.otherRigidBodyNames[i]])
            npRotation = np.array(self.dictRotation[self.otherRigidBodyNames[i]])
            npRigidBodyTransform = np.concatenate([npPosition, npRotation], axis=1)
            npTimeRigidBodyTransform = np.c_[npDuration,npRigidBodyTransform]

            self.ExportAsCSV(npTimeRigidBodyTransform, dirPath, 'Transform_'+self.otherRigidBodyNames[i], participant, conditions,number, transformHeader)
        
        print('Writing: Gripper value...')
        for i in tqdm.tqdm(range(self.bendingSensorNum), ncols=150):
            npBendingSensorValue = np.array(self.dictGripperValue['gripperValue'+str(i+1)])
            self.ExportAsCSV(npBendingSensorValue, dirPath, 'GripperValue_'+str(i+1), participant, conditions,number, bendingSensorHeader)
        
        print('---------- Export completed ----------\n')

        
    
    def ExportAsCSV(self, data, dirPath, fileName, participant, conditions, number, header: list = []):
        """
        Export the data to CSV file.

        Parameters
        ----------
        data: array like
            Data to be exported.
        dirPath: str
            Directory path (not include the file name).
        fileName: str
            File name. (not include ".csv")
        header: (Optional) list
            Header of CSV file. If list is empty, CSV file not include header.
        """
        # ----- Check directory ----- #
        self.mkdir(dirPath)

        exportPath = dirPath + '/' + participant + '_' + conditions + '_' + number + '_' + fileName + '_' + datetime.datetime.now().strftime('%Y%m%d_%H%M') + '.csv'
        with open(exportPath, 'w', newline='') as f:
            writer = csv.writer(f)

            if header:
                writer.writerow(header)
            writer.writerows(data)
    
    def mkdir(self, path):
        """
        Check existence of the directory, and if it does not exist, create a new one.

        Parameters
        ----------
        path: str
            Directory path
        """
        
        if not os.path.isdir(path):
            os.makedirs(path)
