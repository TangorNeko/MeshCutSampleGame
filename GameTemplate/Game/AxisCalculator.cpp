#include "stdafx.h"
#include "AxisCalculator.h"

/// <summary>
/// Rr@ðp¢Än~nsñÌÅLlÆÅLxNgðßéB
/// ºLÌy[WÌvOðpB
/// </summary>
/// http://www.slis.tsukuba.ac.jp/~fujisawa.makoto.fu/cgi-bin/wiki/index.php?%B8%C7%CD%AD%C3%CD/%B8%C7%CD%AD%A5%D9%A5%AF%A5%C8%A5%E
/// <param name="a">ÀÎÌsñDvZãCÎpvfÉÅLlªüé</param>
/// <param name="v">ÅLxNgBaÆ¯¶TCY</param>
/// <param name="eps">û©ë·</param>
/// <param name="iter_max">½ÌÅå</param>
/// <returns></returns>
#if 1
#define   TOL     1.0e-10   
#define   N       3
#define   MAX     100
template<int n>
int EigenJacobiMethod(float a[], float x[])
{
    int    i, j, k, m, count, status;
    float amax, amax0, theta, co, si, co2, si2, cosi, pi = 4.0f * atan(1.0f);
    float aii, aij, ajj, aik, ajk;

    //   úlÝè
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (i == j)  x[n * i + j] = 1.0f; else  x[n * i + j] = 0.0f;
        }
    }

    //   ½vZ
    count = 0;  status = 9;
    while (count <= MAX) {
        //  ñÎpvfÌÅålðTõ
        amax = 0.0;
        for (k = 0; k < n - 1; k++) {
            for (m = k + 1; m < n; m++) {
                amax0 = fabs(a[n * k + m]);
                if (amax0 > amax) { i = k;  j = m, amax = amax0; }
            }
        }
        //  û©»è
        if (amax <= TOL) { status = 0;   break; }
        else {
            aii = a[n * i + i];   aij = a[n * i + j];   ajj = a[n * j + j];
            //   ñ]pxvZ
            if (fabs(aii - ajj) < TOL) {
                theta = 0.25f * pi * aij / fabs(aij);
            }
            else {
                theta = 0.5f * atan(2.0f * aij / (aii - ajj));
            }
            co = cos(theta); si = sin(theta); co2 = co * co; si2 = si * si; cosi = co * si;

            //   Ï·sñ
            a[n * i + i] = co2 * aii + 2.0f * cosi * aij + si2 * ajj;
            a[n * j + j] = si2 * aii - 2.0f * cosi * aij + co2 * ajj;
            a[n * i + j] = 0.0f;    a[n * j + i] = 0.0f;
            for (k = 0; k < n; k++) {
                if (k != i && k != j) {
                    aik = a[n * k + i];            ajk = a[n * k + j];
                    a[n * k + i] = co * aik + si * ajk;  a[n * i + k] = a[n * k + i];
                    a[n * k + j] = -si * aik + co * ajk;  a[n * j + k] = a[n * k + j];
                }
            }

            //   ÅLxNg
            for (k = 0; k < n; k++) {
                aik = x[n * k + i];   ajk = x[n * k + j];
                x[n * k + i] = co * aik + si * ajk;
                x[n * k + j] = -si * aik + co * ajk;
            }
            count++;
        }
    }
    return status;
}
#else
template<int n>
int EigenJacobiMethod(float* a, float* v, float eps = 1e-8, int iter_max = 100)
{
    float bim[n];
    float bjm[n];
    float bii, bij, bjj, bji;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            v[i * n + j] = (i == j) ? 1.0 : 0.0;
        }
    }

    int cnt = 0;
    for (;;) {
        int i = 0;
        int j = 0;

        float x = 0.0;
        for (int ia = 0; ia < n; ++ia) {
            for (int ja = 0; ja < n; ++ja) {
                int idx = ia * n + ja;
                if (ia != ja && fabs(a[idx]) > x) {
                    i = ia;
                    j = ja;
                    x = fabs(a[idx]);
                }
            }
        }

        float aii = a[i * n + i];
        float ajj = a[j * n + j];
        float aij = a[i * n + j];

        float alpha, beta;
        alpha = (aii - ajj) / 2.0;
        beta = sqrt(alpha * alpha + aij * aij);

        float st, ct;
        ct = sqrt((1.0 + fabs(alpha) / beta) / 2.0);    // sinÆ
        st = (((aii - ajj) >= 0.0) ? 1.0 : -1.0) * aij / (2.0 * beta * ct);    // cosÆ

        // A = PAPÌvZ
        for (int m = 0; m < n; ++m) {
            if (m == i || m == j) continue;

            float aim = a[i * n + m];
            float ajm = a[j * n + m];

            bim[m] = aim * ct + ajm * st;
            bjm[m] = -aim * st + ajm * ct;
        }

        bii = aii * ct * ct + 2.0 * aij * ct * st + ajj * st * st;
        bij = 0.0;

        bjj = aii * st * st - 2.0 * aij * ct * st + ajj * ct * ct;
        bji = 0.0;

        for (int m = 0; m < n; ++m) {
            a[i * n + m] = a[m * n + i] = bim[m];
            a[j * n + m] = a[m * n + j] = bjm[m];
        }
        a[i * n + i] = bii;
        a[i * n + j] = bij;
        a[j * n + j] = bjj;
        a[j * n + i] = bji;

        // V = PVÌvZ
        for (int m = 0; m < n; ++m) {
            float vmi = v[m * n + i];
            float vmj = v[m * n + j];

            bim[m] = vmi * ct + vmj * st;
            bjm[m] = -vmi * st + vmj * ct;
        }
        for (int m = 0; m < n; ++m) {
            v[m * n + i] = bim[m];
            v[m * n + j] = bjm[m];
        }

        float e = 0.0;
        for (int ja = 0; ja < n; ++ja) {
            for (int ia = 0; ia < n; ++ia) {
                if (ia != ja) {
                    e += fabs(a[ja * n + ia]);
                }
            }
        }
        if (e < eps) break;

        cnt++;
        if (cnt > iter_max) break;
    }

    return cnt;
}
#endif
Vector3 AxisCalculator::CalcCenterPosition(const std::vector<Vector3>& posArray)
{
    // Ü¸ÍAAABBÌSÀWðßéB
    Vector3 centerPos;
    for (const auto& position : posArray) {
        centerPos += position;
    }
    centerPos /= static_cast<float>(posArray.size());
    return centerPos;
}
void AxisCalculator::CalcCovarianceMatrixFromPositionList(
    float covarianceMatrix[3][3],
    const std::vector<Vector3>& posArray,
    const Vector3& centerPos
)
{
    // 0Åú»·éB
    memset(covarianceMatrix, 0, sizeof(float[3][3]));
    // ¤ªUsñðvZ·éB
    // ¤ªUÆÍXÆYÆZÌvfªÇê­ç¢ÖAÃ¢ÄªUµÄ¢é©ð\·àÌB
    // ¤ªUsñÍA»êðsñÆµÄÜÆß½àÌB
    for (const auto& position : posArray) {
        const auto& aabbCenterPos = position;
        covarianceMatrix[0][0] += (aabbCenterPos.x - centerPos.x) * (aabbCenterPos.x - centerPos.x);
        covarianceMatrix[0][1] += (aabbCenterPos.x - centerPos.x) * (aabbCenterPos.y - centerPos.y);
        covarianceMatrix[1][0] = covarianceMatrix[0][1];

        covarianceMatrix[1][1] += (aabbCenterPos.y - centerPos.y) * (aabbCenterPos.y - centerPos.y);
        covarianceMatrix[0][2] += (aabbCenterPos.x - centerPos.x) * (aabbCenterPos.z - centerPos.z);
        covarianceMatrix[2][0] = covarianceMatrix[0][2];

        covarianceMatrix[2][2] += (aabbCenterPos.z - centerPos.z) * (aabbCenterPos.z - centerPos.z);
        covarianceMatrix[1][2] += (aabbCenterPos.y - centerPos.y) * (aabbCenterPos.z - centerPos.z);
        covarianceMatrix[2][1] = covarianceMatrix[1][2];
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            covarianceMatrix[i][j] /= static_cast<float>(posArray.size());
        }
    }
}

void AxisCalculator::CalcCapsuleAxisFromCovarianceMatrix(
    Vector3& newAxis,
    float covarianceMatrix[3][3],
    const Vector3& centerPos,
    const std::vector<Vector3>& posArray
)
{
    // ¤ªUsñªvZÅ«½ÌÅARr@ðp¢ÄÅLlÆÅLxNgðßéB
    Vector3 eigenVector[3];
    EigenJacobiMethod<3>(
        reinterpret_cast<float*>(covarianceMatrix),
        reinterpret_cast<float*>(&eigenVector)
        );

    // 1ÔÚå«ÈÅLlÌÅLxNgðJvZÌY²Æ·éB
    float eigenScalar_0 = fabsf(covarianceMatrix[0][0]);
    float eigenScalar_1 = fabsf(covarianceMatrix[1][1]);
    float eigenScalar_2 = fabsf(covarianceMatrix[2][2]);

    //NOTE:±±Å@ü(JvZÌü«)ðèµÄ¢é
    if (eigenScalar_0 > eigenScalar_1 && eigenScalar_0 > eigenScalar_2) {
        newAxis.x = eigenVector[0].x;
        newAxis.y = eigenVector[1].x;
        newAxis.z = eigenVector[2].x;
    }
    else if (eigenScalar_1 > eigenScalar_0 && eigenScalar_1 > eigenScalar_2) {
        newAxis.x = eigenVector[0].y;
        newAxis.y = eigenVector[1].y;
        newAxis.z = eigenVector[2].y;
    }
    else if (eigenScalar_2 > eigenScalar_0 && eigenScalar_2 > eigenScalar_1) {
        newAxis.x = eigenVector[0].z;
        newAxis.y = eigenVector[1].z;
        newAxis.z = eigenVector[2].z;
    }
    if (newAxis.Length() < 0.1f) {
        // Rr@Å@üªvZÅ«È©Á½B

        newAxis = posArray.back() - posArray.front();

        newAxis.Normalize();
    }
    // ª½ÊÜÅÌ£ÍSÀWÜÅÌ£Æ·éB
    float distance = Dot(newAxis, centerPos);
    if (distance < 0.0f) {
        newAxis *= -1.0f;
        distance = fabsf(distance);
    }
}

Vector3 AxisCalculator::CalcAxis(const std::vector<Vector3>& posArray)
{
    // å¬ªªÍðsÁÄAª½ÊðßéB


    // Ü¸ÍAÀWQÌSÀWðvZ·éB
    //«
    //TODO:fÌCfbNXobt@©çæ¾µ½ÀWð³É
    //½ÏlðvZ·é

    //S_ð¸çµ½ÌÅVector3::ZeroÉÈé
    //Vector3 centerPos = CalcCenterPosition(posArray);
    Vector3 centerPos = Vector3::Zero;

    // ±¢Ä¤ªUsñðvZ·é
    float covarianceMatrix[3][3];
    CalcCovarianceMatrixFromPositionList(covarianceMatrix, posArray, centerPos);

    //@üA^WFgA]@üó¯æèpÌzñðé¾
    Vector3 newAxis;

    // ¤ªUsñðpµÄJvZÌ²ðvZ·é
    CalcCapsuleAxisFromCovarianceMatrix(newAxis, covarianceMatrix, centerPos, posArray);

    return newAxis;
}