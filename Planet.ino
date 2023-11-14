// A Hold pozíciójának kiszámítása
void calculateMoonPosition(double LMST, int hour, int minute, int second, double Latitude) {
  double T = (double)hour + (double)minute / 60.0 + (double)second / 3600.0;
  T = T / 24.0;
  double GMST = LMST + T * 1.002737909;
  GMST = fmod(GMST, 24.0);

  double M = 6.28875 * DR + 0.017201977 * DR * (T * 36525);
  M = fmod(M, 2 * PI);

  double L = 4.8950630 * DR + 0.017201977 * DR * (T * 36525);
  L = fmod(L + K1 * hour, 2 * PI);

  double MP = 6.283185308 * DR;
  double D = 0.827361 * DR * sin(M) + 0.022421 * DR * sin(2 * L - M) + 0.033863 * DR * sin(2 * L) + 0.049867 * DR * sin(4 * L - M) + 0.050192 * DR * sin(2 * M) + 0.085581 * DR * sin(2 * L + M) + 0.088277 * DR * sin(2 * L + 2 * M) + 0.536065 * DR * sin(2 * L - 2 * M) + 0.591070 * DR * sin(2 * L - 2 * MP) + 0.619360 * DR * sin(2 * L + 2 * MP) - 0.642388 * DR * sin(3 * M) + 0.710978 * DR * sin(2 * L - 3 * M) + 0.871536 * DR * sin(3 * MP) + 0.986007 * DR * sin(3 * L - M) - 1.151804 * DR * sin(3 * L) + 0.607950 * DR * sin(3 * L + M) - 0.764383 * DR * sin(4 * L) - 0.800138 * DR * sin(MP + M) + 0.839956 * DR * sin(4 * L - M) + 2.814985 * DR * sin(4 * L - 2 * M) - 0.983877 * DR * sin(4 * L + M) + 1.965080 * DR * sin(4 * MP) - 1.825494 * DR * sin(4 * L - 2 * MP) - 2.580714 * DR * sin(4 * L + 2 * MP) + 2.781941 * DR * sin(4 * L - MP) + 2.824298 * DR * sin(4 * L + MP) + 5.753384 * DR * sin(4 * L + 2 * M) + 6.272957 * DR * sin(4 * L + 2 * MP) + 6.768074 * DR * sin(4 * L + 4 * M) + 10.165863 * DR * sin(4 * L + 4 * MP) - 0.034700 * DR * sin(0) + 0.023499 * DR * sin(2 * L + 4 * M) - 0.023127 * DR * sin(0 - M) + 0.021600 * DR * sin(4 * L + MP - M) - 0.019767 * DR * sin(2 * L + 2 * MP - M) + 0.018150 * DR * sin(6 * M) + 0.015501 * DR * sin(4 * L - 4 * M) + 0.015215 * DR * sin(0 + M) + 0.014497 * DR * sin(4 * L + MP + M) + 0.013857 * DR * sin(4 * L - 2 * MP + M) - 0.013642 * DR * sin(0 - 2 * M) - 0.012667 * DR * sin(2 * L + 2 * MP + M) - 0.012584 * DR * sin(2 * L - M) + 0.011836 * DR * sin(2 * L + M - 2 * MP) + 0.011428 * DR * sin(4 * L - 2 * M - MP) + 0.011032 * DR * sin(4 * L - 3 * M) + 0.010752 * DR * sin(4 * L - M + MP) + 0.010548 * DR * sin(2 * L + 2 * MP + 2 * M) + 0.010464 * DR * sin(4 * L + M - MP) + 0.009930 * DR * sin(3 * L + M) - 0.009872 * DR * sin(4 * L + M + MP) - 0.009847 * DR * sin(4 * L + 2 * MP - M) - 0.008281 * DR * sin(4 * L - 3 * M + MP) - 0.007753 * DR * sin(4 * L - 3 * MP) + 0.006864 * DR * sin(4 * L - 2 * M + MP) + 0.006650 * DR * sin(2 * L + 2 * M - 2 * MP) - 0.006644 * DR * sin(2 * L + 2 * MP - 2 * M) - 0.006192 * DR * sin(4 * L + 2 * MP + M) + 0.005804 * DR * sin(2 * L + 2 * M + 2 * MP) + 0.005159 * DR * sin(4 * L + 2 * M - MP) + 0.005078 * DR * sin(0 + 2 * M) - 0.004049 * DR * sin(2 * L + MP + M) - 0.004012 * DR * sin(2 * L + MP - M) - 0.003996 * DR * sin(4 * L - MP - M) - 0.003862 * DR * sin(0 + 2 * MP) + 0.003665 * DR * sin(2 * L - MP - M) + 0.003631 * DR * sin(2 * L - 4 * M) - 0.003019 * DR * sin(2 * L + M - 2 * MP) + 0.002870 * DR * sin(4 * L + 4 * MP - M) - 0.002768 * DR * sin(4 * L + M - 2 * MP) + 0.002730 * DR * sin(2 * L + 2 * MP - 2 * M) + 0.002580 * DR * sin(4 * L + 4 * M) - 0.002567 * DR * sin(2 * L - 4 * MP) - 0.002521 * DR * sin(4 * L + 2 * M + MP) - 0.002485 * DR * sin(4 * L + 4 * MP) - 0.002344 * DR * sin(2 * L + 3 * M) + 0.002236 * DR * sin(4 * L - 4 * MP) + 0.002086 * DR * sin(4 * L - M - MP) + 0.001964 * DR * sin(4 * L + 4 * M - MP) - 0.001903 * DR * sin(0 - 2 * MP) + 0.001874 * DR * sin(4 * L + M - 2 * MP) - 0.001828 * DR * sin(4 * L - M + 2 * MP) - 0.001803 * DR * sin(0 - 2 * M) + 0.001750 * DR * sin(0 + 2 * MP) - 0.001570 * DR * sin(4 * L - 4 * M - MP) - 0.001487 * DR * sin(4 * L + 2 * M - 2 * MP) + 0.001481 * DR * sin(4 * L + 4 * MP - 2 * M) + 0.001417 * DR * sin(4 * L + 4 * M + MP) + 0.001350 * DR * sin(2 * L + M + 2 * MP) + 0.001330 * DR * sin(4 * L + 4 * MP - 2 * M) + 0.001106 * DR * sin(4 * L - 4 * M + MP) + 0.000963 * DR * sin(2 * L + M + 3 * M) - 0.000916 * DR * sin(4 * L - 2 * M + 2 * MP) + 0.000874 * DR * sin(2 * L + 2 * M - 3 * MP) - 0.000853 * DR * sin(4 * L + M - 3 * MP) - 0.000781 * DR * sin(4 * L + M + 2 * MP) + 0.000670 * DR * sin(4 * L + 2 * M - 3 * MP) + 0.000606 * DR * sin(4 * L - M - 3 * MP) - 0.000597 * DR * sin(2 * L + 4 * M - 2 * MP) - 0.000561 * DR * sin(4 * L + 2 * M + 2 * MP) - 0.000546 * DR * sin(4 * L + 4 * MP + M) + 0.000539 * DR * sin(4 * L + 4 * MP + M) - 0.000520 * DR * sin(4 * L - 4 * MP + M) - 0.000487 * DR * sin(4 * L - M - 2 * MP) + 0.000453 * DR * sin(4 * L + M + 3 * MP) + 0.000439 * DR * sin(4 * L + 2 * M + 3 * MP) + 0.000423 * DR * sin(2 * L + 3 * M - MP) - 0.000422 * DR * sin(4 * L - 2 * M + 3 * MP) + 0.000408 * DR * sin(4 * L + 2 * M + 2 * MP) + 0.000392 * DR * sin(4 * L + 2 * M + 4 * MP) + 0.000377 * DR * sin(2 * L + 4 * MP - M) - 0.000347 * DR * sin(4 * L - 2 * M + 4 * MP) - 0.000340 * DR * sin(2 * L + 4 * MP + M) + 0.000330 * DR * sin(4 * L + 4 * MP - 3 * M) - 0.000317 * DR * sin(2 * L + M - 4 * MP) + 0.000317 * DR * sin(4 * L - 3 * M - MP) + 0.000306 * DR * sin(2 * L + M - 3 * MP) + 0.000283 * DR * sin(4 * L + 2 * MP - 2 * M) + 0.000280 * DR * sin(2 * L + 2 * M + 4 * MP) - 0.000279 * DR * sin(4 * L + 4 * MP + 2 * M) - 0.000264 * DR * sin(4 * L + M + 4 * MP) + 0.000252 * DR * sin(2 * L + 2 * MP + 3 * M) + 0.000250 * DR * sin(4 * L + 4 * M - 2 * MP) - 0.000238 * DR * sin(4 * L - M + 4 * MP) + 0.000227 * DR * sin(2 * L + 2 * M - 4 * MP) + 0.000227 * DR * sin(4 * L - 2 * M - 2 * MP) + 0.000217 * DR * sin(4 * L + 2 * MP + 3 * M) - 0.000209 * DR * sin(4 * L + M + 4 * MP) + 0.000206 * DR * sin(2 * L + 2 * MP + 4 * M) - 0.000189 * DR * sin(4 * L + 4 * M + 2 * MP) + 0.000182 * DR * sin(4 * L - M + 3 * MP) - 0.000180 * DR * sin(4 * L + 4 * MP + 3 * M) - 0.000177 * DR * sin(4 * L - M - 3 * MP) + 0.000173 * DR * sin(4 * L - 4 * M - 2 * MP) - 0.000170 * DR * sin(2 * L + 2 * MP - 4 * M) + 0.000159 * DR * sin(4 * L + 2 * M - 4 * MP) + 0.000157 * DR * sin(4 * L + 4 * MP - 4 * M) + 0.000155 * DR * sin(4 * L + 2 * MP - 4 * M) - 0.000151 * DR * sin(4 * L + 4 * MP + 4 * M) + 0.000150 * DR * sin(4 * L + 4 * M - 3 * MP) - 0.000146 * DR * sin(2 * L + 4 * MP - 3 * M) + 0.000139 * DR * sin(2 * L + 4 * M - 3 * MP) - 0.000139 * DR * sin(4 * L - 2 * MP + 4 * M) - 0.000139 * DR * sin(2 * L + 2 * MP + 4 * MP) + 0.000136 * DR * sin(4 * L - 2 * MP - 4 * M) + 0.000131 * DR * sin(4 * L + M - 4 * MP) - 0.000125 * DR * sin(4 * L + M - 4 * M) - 0.000110 * DR * sin(4 * L + 2 * M + 4 * MP) - 0.000105 * DR * sin(4 * L + 4 * M + 3 * MP) + 0.000101 * DR * sin(4 * L - 4 * MP - M) - 0.000100 * DR * sin(4 * L + 2 * M - 4 * MP);

  double C = 0.000325 * sin(M) + 0.000165 * sin(MP) + 0.000164 * sin(2 * L - M) + 0.000126 * sin(D) + 0.000110 * sin(2 * L - MP) + 0.000062 * sin(2 * L + M) + 0.000060 * sin(2 * L - D) + 0.000056 * sin(2 * D) + 0.000047 * sin(2 * L + MP) - 0.000042 * sin(3 * M) + 0.000040 * sin(2 * L - 2 * MP) + 0.000037 * sin(3 * MP) + 0.000035 * sin(2 * L + 2 * MP) - 0.000033 * sin(4 * L - M) - 0.000030 * sin(4 * L) + 0.000029 * sin(2 * L + MP) + 0.000026 * sin(2 * L - 3 * M) + 0.000021 * sin(2 * L + 3 * M) - 0.000017 * sin(3 * M) + 0.000016 * sin(2 * L - MP) + 0.000016 * sin(3 * MP) - 0.000014 * sin(MP + M) + 0.000010 * sin(4 * L - M) - 0.000010 * sin(4 * L) + 0.000009 * sin(3 * L - M) + 0.000007 * sin(2 * L + 2 * M) + 0.000006 * sin(2 * L - 4 * M) - 0.000006 * sin(2 * L - D + M) - 0.000005 * sin(2 * D) + 0.000005 * sin(2 * L - 2 * D) + 0.000005 * sin(2 * L + D);

  double sin_DEC = sin(D) * sin(Latitude) + cos(D) * cos(Latitude) * cos(L);
  DEC_my = asin(sin_DEC) / DR;

  double cosH = (sin(0 * DR) - sin(Latitude) * sin_DEC) / (cos(Latitude) * cos(DEC_my));
  double H = acos(cosH) / DR;

  double UT = T * 24.0;
  double T_corr = 0.06571 * UT + GMST - LMST;
  H += T_corr;
  H = fmod(H, 360.0);

  RA = 360.0 - H;
}

// A Mars pozíciójának közelítő kiszámítása
void calculateMarsPosition(double LMST, int year, int month, int day, int hour, int minute, int second, double Latitude, double Longitude) {
  // Dátum és idő átalakítása Julian Date-re
  double JD = getJulianDate(year, month, day, hour, minute, second);

  // Mars bolygó szögsebessége (fokban / nap)
  double MarsAngularVelocity = 0.5240217766;

  // A Mars bolygó átlagos közelsége a Naphoz (1 AU)
  double MarsDistance = 1.0;

  // A Mars bolygó látható (helyi) idő (órákban)
  double MarsLocalTime = LMST + (Longitude / 15.0);

  // Mars bolygó jobboldali ascenziója (RA) kiszámítása
  Mars_RA = fmod(360.0 + (MarsLocalTime * K1) - (MarsAngularVelocity * (JD - 2451545.0)), 360.0);

  // Mars bolygó deklinációja (DEC) kiszámítása
  double MarsSinDeclination = sin((PI / 180) * Latitude) * sin((PI / 180) * MarsDistance) +
                              cos((PI / 180) * Latitude) * cos((PI / 180) * MarsDistance) *
                              cos((PI / 180) * Mars_RA);
  Mars_DEC = (180 / PI) * asin(MarsSinDeclination);
}