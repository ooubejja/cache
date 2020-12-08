clear all; close all; clc;

%P_tx = 11:-1:0
SNR = 11:-1:0

user_4_ber = [0.00E+00
0.00E+00
0.00E+00
0.00E+00
1.00E-03
1.96E-02
2.27E-02
3.10E-02
4.83E-02
1.21E-01
2.17E-01
8.47E-01
];
user_0_ber = [0.00E+00
0.00E+00
0.00E+00
0.00E+00
0.00E+00
1.81E-03
6.45E-03
1.21E-02
1.29E-02
2.53E-02
5.13E-02
2.07E-01
];

user_4_thr = [2.89E+01
2.86E+01
2.87E+01
2.89E+01
2.89E+01
2.90E+01
2.87E+01
2.86E+01
2.86E+01
2.79E+01
2.57E+01
5.25E+00
]/80;
user_0_thr = [2.60E+01
2.60E+01
2.62E+01
2.57E+01
2.58E+01
2.60E+01
2.60E+01
2.61E+01
2.59E+01
2.58E+01
2.58E+01
2.29E+01
]/80;

figure(1)
grid on; hold on;
color = get (gca, "ColorOrder");

[hax, a0, b0] = plotyy (SNR, user_0_ber, SNR, user_0_thr,@semilogy,@plot);
[~, a4, b4] = plotyy (SNR, user_4_ber, SNR, user_4_thr,@semilogy,@plot);


% Color Style by User
set ([a0, b0], "marker", "o", 'LineWidth',2, "color", color(1,:));
set ([a4, b4], "marker", "o", 'LineWidth',2, "color", color(4,:));

% Line Style by metric
set ([a0, a4], "linestyle", "-");
set ([b0, b4], "linestyle", "-.");

set(hax,{'ycolor'},{'k';'k'})
grid on;
xlabel (hax(1), "xlabel");
ylabel (hax(1), "BER");
ylabel (hax(2), "Throughput (bits/channel use)");

xlabel('SNR');
hleg1 = legend('Weak User BER','Strong User BER','Weak User Throughput','Strong User Throughput',"location", "southeast");
%title (hax(2), 'BER vs Throughput | 5 Users | 3 Weak + 2 Strong | Rw = 1/4 | Rs = 1/2');
title (hax(2), 'Wise BER vs Throughput | 5 Users | 3 Weak + 2 Strong | CodeRate=(1,2,3,4,4)');

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%