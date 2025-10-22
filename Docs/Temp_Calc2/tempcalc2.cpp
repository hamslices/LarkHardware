/*
    Thermistor Maths
    HamSlices 4/3/2024
*/

#include <iostream>
#include <cstdint>
#include <cstdio>
#include <cmath>

#define BETA       3950.0  //for 30k thermistor?
#define AMB_TMP    25.0    //c
#define AMB_RES    30000.0 //30k
#define VCC        3.3     //v

double c_to_k(double c);
double calc_resistance_at_c(double c);

//                vout
//                 |
// vcc|----V^rs^V-----V^ntc^V------|gnd 
//
double vout_at_c(double rs, double c);
void solve_comp(double tl, double th, double rh, double rs);


int main()
{
    double target_temp_low = 40.0; // c
    double target_temp_high = 80.0; // c
    double series_r = 3.0 * 1000;
    double hyst_r = 100.0 * 1000;

    solve_comp(target_temp_low, target_temp_high, hyst_r, series_r);

    return 0;
}

double calc_resistance_at_c(double c)
{
    double alpha = ((c_to_k(c) * c_to_k(AMB_TMP)) / (c_to_k(c) - c_to_k(AMB_TMP)));
    return AMB_RES / exp(BETA / alpha);
}

double c_to_k(double c)
{
    return c + 273.15;
}

double vout_at_c(double rs, double c)
{
    double rth = calc_resistance_at_c(c);
    return VCC * (rth / (rth + rs));
}

void solve_comp(double tl, double th, double rh, double rs)
{
    double r_l = calc_resistance_at_c(tl);
    double r_h = calc_resistance_at_c(th);

    double vl = vout_at_c(rs, th);
    double vh = vout_at_c(rs, tl);

    double rx = ((vh - vl) * rh) / vl;
    double ry = ((vl * rx) / (VCC - vh));

    printf("--------------------------------\n");
    printf("VCC          : %.2f\n", VCC);
    printf("BETA         : %.2f\n", BETA);
    printf("AMB_TMP      : %.2f\n", AMB_TMP);
    printf("AMB_NTC_RES  : %.2f\n", AMB_RES);
    printf("NTC series R : %.2f\n", rs);
    printf("--------------------------------\n");
    printf("R_x = %.2f\n", rx);
    printf("R_y = %.2f\n", ry);
    printf("R_h = %.2f\n", rh);
    printf("--------------------------------\n");
    printf("--- Test Point Values ----------\n");
    printf("ntc_r: %.2f @ %.2fC, ntc_v = %.2f\n", r_h, th, vout_at_c(rs, th));
    printf("ntc_r: %.2f @ %.2fC, ntc_v = %.2f\n", r_l, tl, vout_at_c(rs, tl));
    printf("--------------------------------\n");
}
