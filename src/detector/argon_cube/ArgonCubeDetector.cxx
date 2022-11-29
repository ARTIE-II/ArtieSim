/**
 * @file ArgonCubeDetector.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @details 
 *  Change log:
 *      2022/09/20 - Initial creation of the file.
 * @date 2022-09-23
 */
#include "ArgonCubeDetector.hh"

namespace Artie
{
    ArgonCubeDetector::ArgonCubeDetector(
        G4double CubeX,
        G4double CubeY,
        G4double CubeZ,
        G4int BottomNumberX,
        G4int BottomNumberZ,
        G4double BottomThicknessY
    )
    : Detector()
    , mCubeX(CubeX)
    , mCubeY(CubeY)
    , mCubeZ(CubeZ)
    , mBottomNumberX(BottomNumberX)
    , mBottomNumberZ(BottomNumberZ)
    , mBottomThicknessY(BottomThicknessY)
    {
        /// create the argon cube active volume
        std::shared_ptr<ArgonCubeActiveVolume> ActiveVolume 
            = std::make_shared<ArgonCubeActiveVolume>(
            mCubeX, 
            mCubeY,
            mCubeZ
        );
        AddDetectorComponent(ActiveVolume);

        // add the bottom sipm array
        for (G4int ii = 0; ii < mBottomNumberX; ii++)
        {
            for (G4int jj = 0; jj < mBottomNumberZ; jj++)
            {
                std::shared_ptr<ArgonCubeXZSiPM> XZSiPM = std::make_shared<ArgonCubeXZSiPM>(
                    ii + jj * mBottomNumberZ,
                    (mCubeX)/mBottomNumberX,
                    mBottomThicknessY/2.0,
                    (mCubeZ)/mBottomNumberZ,
                    G4ThreeVector(
                        -(mCubeX) + (mCubeX)/mBottomNumberX + ii * (2 * mCubeX)/mBottomNumberX,
                        -(mCubeY + mBottomThicknessY/2.0),
                        -(mCubeZ) + (mCubeZ)/mBottomNumberZ + jj * (2 * mCubeZ)/mBottomNumberZ
                    )
                );
                AddDetectorComponent(XZSiPM);
            }
        }
    }

    ArgonCubeDetector::~ArgonCubeDetector()
    {
    }
}