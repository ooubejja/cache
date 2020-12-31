clear all; close all; clc;
% WISE

%P_tx = 11:-1:0
SNR = 11:-1:0

user_4_ber = [1.11E-02
1.35E-02
2.01E-02
2.39E-02
3.26E-02
6.19E-02
1.11E-01
1.51E-01
1.53E-01
5.85E-01
7.94E-01
9.42E-01
];
user_0_ber = [1.12E-02
2.47E-02
2.90E-02
2.96E-02
3.78E-02
6.28E-02
6.66E-02
8.27E-02
9.99E-02
1.23E-01
2.07E-01
5.10E-01
];
user_1_ber = [1.26E-02
1.96E-02
2.14E-02
4.03E-02
4.24E-02
6.55E-02
9.56E-02
1.26E-01
1.56E-01
1.62E-01
2.52E-01
5.06E-01
];

thput = [1.92E-01
1.64E-01
1.54E-01
1.55E-01
1.50E-01
1.45E-01
1.39E-01
1.27E-01
1.25E-01
9.38E-02
6.05E-02
3.76E-02
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
%title (hax(2), 'BER vs Throughput in Dynamic Fading Channel | 5 Users: 3 Weak + 2 Strong | Code rates=[1/6,1/3,1/2,2/3,2/3]');
title (hax(2), {"BER vs Throughput in a dynamic fading channel"; "5 Users: 3 Weak + 2 Strong | Code rates=[1/6,1/3,1/2,2/3,2/3]"}, "fontsize", 8);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%