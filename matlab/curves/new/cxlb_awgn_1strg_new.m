clear all; close all; clc;
% WISE

%P_tx = 11:-1:0
SNR = 11:-1:0

user_4_ber = [0.00E+00
0.00E+00
0.00E+00
3.77E-03
6.43E-03
1.09E-02
1.30E-02
2.17E-02
4.90E-02
7.37E-01
9.34E-01
9.57E-01

];
user_0_ber = [0.00E+00
0.00E+00
0.00E+00
0.00E+00
0.00E+00
2.60E-03
4.51E-03
6.73E-03
1.24E-02
2.17E-02
4.07E-02
1.59E-01
];
user_1_ber = [0.00E+00
0.00E+00
0.00E+00
1.77E-03
9.36E-03
1.61E-02
2.63E-02
3.04E-02
5.66E-02
1.60E-01
1.89E-01
3.26E-01
];

thput = [2.13E-01
1.99E-01
1.75E-01
1.54E-01
1.27E-01
1.22E-01
1.17E-01
1.16E-01
1.11E-01
7.52E-02
5.68E-02
4.62E-02
];

hax = gca;
figure(1)
grid on; hold on;
color = get (gca, "ColorOrder");

[hax, tmp, b0] = plotyy (hax,0,0, SNR, thput,@semilogy,@plot);
delete(tmp);
%[hax, a0, b0] = plotyy (hax,0,0, SNR, thput,@semilogy,@semilogy);
%[~, a0] = semilogy (SNR, user_0_ber);
%[~, a4] = semilogy (SNR, user_4_ber);
%[~, a1] = semilogy (SNR, user_1_ber);
a0 = semilogy (SNR, user_0_ber);
a1 = semilogy (SNR, user_1_ber);
a4 = semilogy (SNR, user_4_ber);

% Color Style by User
set ([a0], "marker", "o", 'LineWidth',2, "color", color(1,:));
set ([a1], "marker", "o", 'LineWidth',2, "color", color(2,:));
set ([a4], "marker", "o", 'LineWidth',2, "color", color(4,:));
set ([b0], "marker", "o", 'LineWidth',2, "color", color(5,:));
%set ([a1, b1], "marker", "o", 'LineWidth',2, "color", color(2,:));

% Line Style by metric
set ([a0, a4, a1], "linestyle", "-");
set ([b0], "linestyle", "-.");
%set ([b0, b4], "linestyle", "-.");

set(hax,{'ycolor'},{'k';'k'})
grid on;
xlabel (hax(1), "xlabel");
ylabel (hax(1), "BER");
ylabel (hax(2), "Throughput (bits/channel use)");

xlabel('SNR');
hleg1 = legend('Weak User, CR=1/6','Weak User, CR=1/3','Strong User, CR=2/3','Total Throughput',"location", "southeast");
%title (hax(2), 'BER vs Throughput | 5 Users | 3 Weak + 2 Strong | Rw = 1/4 | Rs = 1/2');
%title (hax(2), 'BER vs Throughput | 5 Users: 4 Weak + 1 Strong | Code rates=(1/6,1/3,1/3,1/2,2/3)');
title (hax(2), {"BER vs Throughput"; "5 Users: 4 Weak + 1 Strong | Code rates=[1/6,1/3,1/3,1/2,2/3]"}, "fontsize", 8);


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%