clear all; close all; clc;
% WISE

%P_tx = 11:-1:0
SNR = 11:-1:0

user_4_ber = [8.32E-03
1.37E-02
2.38E-02
3.82E-02
3.87E-02
4.24E-02
4.26E-02
8.48E-02
3.17E-01
5.39E-01
8.78E-01
9.49E-01
];
user_0_ber = [3.28E-03
9.13E-03
1.14E-02
1.14E-02
3.82E-02
3.65E-02
5.75E-02
7.28E-02
7.39E-02
1.01E-01
2.10E-01
3.41E-01
];
user_1_ber = [0.00E+00
4.86E-03
4.93E-03
3.50E-02
3.90E-02
4.97E-02
5.52E-02
7.48E-02
1.12E-01
2.65E-01
2.69E-01
4.65E-01
];

thput = [1.49E-01
1.45E-01
1.44E-01
1.38E-01
1.35E-01
1.27E-01
1.23E-01
1.13E-01
8.15E-02
2.97E-02
7.87E-03
3.29E-03
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
set ([a0, a1, a4], "linestyle", "-");
set ([b0], "linestyle", "-.");
%set ([b0, b4], "linestyle", "-.");

set(hax,{'ycolor'},{'k';'k'})
grid on;
xlabel (hax(1), "xlabel");
ylabel (hax(1), "BER");
ylabel (hax(2), "Throughput (bits/channel use)");

xlabel('SNR');
hleg1 = legend('Weak User, CR=1/6','Weak User, CR=1/3','Strong User, CR=2/3','Total Throughput',"location", "southeast");
%title (hax(2), cstrcat("BER vs Throughput in Dynamic Fading Channel \n5 Users: 4 Weak + 1 Strong | Code rates=[1/6,1/3,1/3,1/2,2/3]"));
title (hax(2), {"BER vs Throughput in a dynamic fading channel"; "5 Users: 4 Weak + 1 Strong | Code rates=[1/6,1/3,1/3,1/2,2/3]"}, "fontsize", 8);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%