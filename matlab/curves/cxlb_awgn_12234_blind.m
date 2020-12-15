clear all; close all; clc;
% BLIND

%P_tx = 11:-1:0
SNR = 12:-1:0

user_4_ber = [0.00E+00
0.00E+00
0.00E+00
0.00E+00
0.00E+00
1.94E-03
1.34E-02
1.39E-02
1.40E-02
4.36E-02
1.44E-01
1.45E-01
1.47E-01
];
user_0_ber = [0.00E+00
2.49E-03
6.70E-03
9.01E-03
8.76E-03
1.25E-02
1.25E-02
1.63E-02
3.42E-02
6.00E-02
5.65E-02
1.15E-01
1.80E-01
];

user_4_thr = [2.70E+01
2.70E+01
2.69E+01
2.69E+01
2.69E+01
2.69E+01
2.66E+01
2.59E+01
2.67E+01
7.54E+00
3.23E+00
2.76E+00
8.14E-01
]/80;
user_0_thr = [2.42E+01
2.42E+01
2.41E+01
2.41E+01
2.41E+01
2.39E+01
2.40E+01
2.37E+01
2.32E+01
2.26E+01
2.18E+01
1.97E+01
1.70E+01
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
title (hax(2), 'Blind BER vs Throughput | 5 Users | 4 Weak + 1 Strong | CodeRate=(1,2,2,3,4)');

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%