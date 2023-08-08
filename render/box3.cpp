
#include "box3.h"

namespace Render
{

bool Box3::isIntersect(const Box3& zBox) const
{
    const REAL fCutoff = REAL(0.999999);
	bool bExistsParallelPair = false;

    const Vector3* azA = m_axis;
    const Vector3* azB = zBox.axes();
    const REAL* afEA = m_extent;
    const REAL* afEB = zBox.extents();

    Vector3 zD = zBox.center() - m_center;

    REAL aafC[3][3];
    REAL aafAbsC[3][3];
    REAL afAD[3];
    REAL fR0;
    REAL fR1;
    REAL fR;
    REAL fR01;

	for(int i = 0; i < 3; ++i)
	{
        aafC[0][i] = azA[0] & azB[i];
        aafAbsC[0][i] = ABSR(aafC[0][i]);
		if(aafAbsC[0][i] > fCutoff)
		{
			bExistsParallelPair = true;
		}
	}

    afAD[0] = azA[0] & zD;
    fR = ABSR(afAD[0]);
    fR1 = afEB[0] * aafAbsC[0][0] + afEB[1] * aafAbsC[0][1] + afEB[2] * aafAbsC[0][2];
	fR01 = afEA[0] + fR1;
	if(fR > fR01)
	{
		return false;
	}

    for(int i = 0; i < 3; ++i)
	{
        aafC[1][i] = azA[1] & azB[i];
        aafAbsC[1][i] = ABSR(aafC[1][i]);
		if(aafAbsC[1][i] > fCutoff)
		{
			bExistsParallelPair = true;
		}
	}

    afAD[1] = azA[1] & zD;
    fR = ABSR(afAD[1]);
    fR1 = afEB[0] * aafAbsC[1][0] + afEB[1] * aafAbsC[1][1] + afEB[2] * aafAbsC[1][2];
	fR01 = afEA[1] + fR1;
	if(fR > fR01)
	{
		return false;
	}

    for(int i = 0; i < 3; ++i)
	{
		aafC[2][i] = azA[2]&azB[i];
        aafAbsC[2][i] = ABSR(aafC[2][i]);
		if(aafAbsC[2][i] > fCutoff)
		{
			bExistsParallelPair = true;
		}
	}

    afAD[2] = azA[2] & zD;
    fR = ABSR(afAD[2]);
    fR1 = afEB[0] * aafAbsC[2][0] + afEB[1] * aafAbsC[2][1] + afEB[2] * aafAbsC[2][2];
	fR01 = afEA[2] + fR1;
	if(fR > fR01)
	{
		return false;
	}

    fR = ABSR(azB[0]&zD);
    fR0 = afEA[0] * aafAbsC[0][0] + afEA[1] * aafAbsC[1][0] + afEA[2] * aafAbsC[2][0];
	fR01 = fR0 + afEB[0];
	if(fR > fR01)
	{
		return false;
	}

    fR = ABSR(azB[1]&zD);
    fR0 = afEA[0] * aafAbsC[0][1] + afEA[1] * aafAbsC[1][1] + afEA[2] * aafAbsC[2][1];
	fR01 = fR0 + afEB[1];
	if(fR > fR01)
	{
		return false;
	}

    fR = ABSR(azB[2] & zD);
    fR0 = afEA[0] * aafAbsC[0][2] + afEA[1] * aafAbsC[1][2] + afEA[2] * aafAbsC[2][2];
	fR01 = fR0 + afEB[2];
	if(fR > fR01)
	{
		return false;
	}

	if(bExistsParallelPair)
	{
		return true;
	}

    fR = ABSR(afAD[2] * aafC[1][0] - afAD[1] * aafC[2][0]);
    fR0 = afEA[1] * aafAbsC[2][0] + afEA[2] * aafAbsC[1][0];
    fR1 = afEB[1] * aafAbsC[0][2] + afEB[2] * aafAbsC[0][1];
	fR01 = fR0 + fR1;
	if(fR > fR01)
	{
		return false;
	}

    fR = ABSR(afAD[2] * aafC[1][1] - afAD[1] * aafC[2][1]);
    fR0 = afEA[1]*aafAbsC[2][1] + afEA[2] * aafAbsC[1][1];
    fR1 = afEB[0]*aafAbsC[0][2] + afEB[2] * aafAbsC[0][0];
	fR01 = fR0 + fR1;
	if(fR > fR01)
	{
		return false;
	}

    fR = ABSR(afAD[2]*aafC[1][2] - afAD[1]*aafC[2][2]);
	fR0 = afEA[1]*aafAbsC[2][2] + afEA[2]*aafAbsC[1][2];
	fR1 = afEB[0]*aafAbsC[0][1] + afEB[1]*aafAbsC[0][0];
	fR01 = fR0 + fR1;
	if(fR > fR01)
	{
		return false;
	}

    fR = ABSR(afAD[0]*aafC[2][0] - afAD[2]*aafC[0][0]);
	fR0 = afEA[0]*aafAbsC[2][0] + afEA[2]*aafAbsC[0][0];
	fR1 = afEB[1]*aafAbsC[1][2] + afEB[2]*aafAbsC[1][1];
	fR01 = fR0 + fR1;
	if(fR > fR01)
	{
		return false;
	}

    fR = ABSR(afAD[0]*aafC[2][1] - afAD[2]*aafC[0][1]);
	fR0 = afEA[0]*aafAbsC[2][1] + afEA[2]*aafAbsC[0][1];
	fR1 = afEB[0]*aafAbsC[1][2] + afEB[2]*aafAbsC[1][0];
	fR01 = fR0 + fR1;
	if(fR > fR01)
	{
		return false;
	}

    fR = ABSR(afAD[0]*aafC[2][2] - afAD[2]*aafC[0][2]);
	fR0 = afEA[0]*aafAbsC[2][2] + afEA[2]*aafAbsC[0][2];
	fR1 = afEB[0]*aafAbsC[1][1] + afEB[1]*aafAbsC[1][0];
	fR01 = fR0 + fR1;
	if(fR > fR01)
	{
		return false;
	}

    fR = ABSR(afAD[1]*aafC[0][0] - afAD[0]*aafC[1][0]);
	fR0 = afEA[0]*aafAbsC[1][0] + afEA[1]*aafAbsC[0][0];
	fR1 = afEB[1]*aafAbsC[2][2] + afEB[2]*aafAbsC[2][1];
	fR01 = fR0 + fR1;
	if(fR > fR01)
	{
		return false;
	}

    fR = ABSR(afAD[1]*aafC[0][1] - afAD[0]*aafC[1][1]);
	fR0 = afEA[0]*aafAbsC[1][1] + afEA[1]*aafAbsC[0][1];
	fR1 = afEB[0]*aafAbsC[2][2] + afEB[2]*aafAbsC[2][0];
	fR01 = fR0 + fR1;
	if(fR > fR01)
	{
		return false;
	}

    fR = ABSR(afAD[1]*aafC[0][2] - afAD[0]*aafC[1][2]);
	fR0 = afEA[0]*aafAbsC[1][2] + afEA[1]*aafAbsC[0][2];
	fR1 = afEB[0]*aafAbsC[2][1] + afEB[1]*aafAbsC[2][0];
	fR01 = fR0 + fR1;
	if(fR > fR01)
	{
		return false;
	}

	return true;
}

//namespace Render
}
