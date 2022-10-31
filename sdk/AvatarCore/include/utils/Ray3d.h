
/********************************************************************
        created:	2008/06/17
        created:	17:6:2008   15:13
        filename: 	i:\Programing\GameEngine\mrayEngine\mrayMath\Ray3d.h
        file path:	i:\Programing\GameEngine\mrayEngine\mrayMath
        file base:	Ray3d
        file ext:	h
        author:		Mohamad Yamen Saraiji

        purpose:
*********************************************************************/

#ifndef ___Ray3d___
#define ___Ray3d___

#include "line3D.h"
#include "point3D.h"

namespace utils {

class Ray3d {
   private:
   protected:
   public:
    vector3d Start;
    vector3d Dir;

    Ray3d() : Start(0, 0, 0), Dir(0, 0, 1){};
    Ray3d(const vector3d &s, const vector3d &d) : Start(s), Dir(d) {}
    Ray3d(const line3d &l) {
        Start = l.pStart;
        Dir = l.getVector();
        Dir.Normalize();
    }
    virtual ~Ray3d() {}

    bool operator==(const Ray3d &l) const {
        return Start == l.Start && Dir == l.Dir;
    }
    bool operator!=(const Ray3d &l) const {
        return Start != l.Start || Dir != l.Dir;
    }
};

}  // namespace utils

#endif  //___Ray___
