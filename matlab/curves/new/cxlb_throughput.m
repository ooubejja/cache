clear all; close all; clc;
% WISE

%P_tx = 11:-1:0
SNR = 11:-1:0

th_awgn_1 = [2.13E-01
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
th_awgn_2 = [2.26E-01
1.70E-01
1.60E-01
1.45E-01
1.38E-01
1.36E-01
1.35E-01
1.30E-01
1.27E-01
9.48E-02
8.07E-02
5.22E-02
];
th_fad_1 = [1.49E-01
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

th_fad_2= [1.92E-01
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

%[hax, tmp, b0] = plotyy (hax,0,0, SNR, thput,@semilogy,@plot);
%delete(tmp);
%[hax, a0, b0] = plotyy (hax,0,0, SNR, thput,@semilogy,@semilogy);
%[~, a0] = semilogy (SNR, user_0_ber);
%[~, a4] = semilogy (SNR, user_4_ber);
%[~, a1] = semilogy (SNR, user_1_ber);
a0 = semilogy (SNR, th_awgn_1);
a1 = semilogy (SNR, th_awgn_2);
a2 = semilogy (SNR, th_fad_1);
a3 = semilogy (SNR, th_fad_2);

% Color Style by User
set ([a0], "marker", "o", 'LineWidth',2, "color", color(1,:));
set ([a1], "marker", "o", 'LineWidth',2, "color", color(2,:));
set ([a2], "marker", "o", 'LineWidth',2, "color", color(4,:));
set ([a3], "marker", "o", 'LineWidth',2, "color", color(5,:));
%set ([a1, b1], "marker", "o", 'LineWidth',2, "color", color(2,:));

% Line Style by metric
%set ([a0, a4, a1], "linestyle", "-");
%set ([b0], "linestyle", "-.");
%set ([b0, b4], "linestyle", "-.");

%set(hax,{'ycolor'},{'k';'k'})
grid on;
xlabel (hax(1), "xlabel");
ylabel (hax(1), "Throughput (bits/channel use)");
%ylabel (hax(2), "Throughput (bits/channel use)");

xlabel('SNR');
hleg1 = legend('Gaussian | 1 Strong 4 Weak','Gaussian | 2 Strong 3 Weak','Dynamic fading | 1 Strong 4 Weak','Dynamic fading | 2 Strong 3 Weak',"location", "southeast");
%title (hax(2), 'BER vs Throughput | 5 Users | 3 Weak + 2 Strong | Rw = 1/4 | Rs = 1/2');
%title (hax(2), 'BER vs Throughput in Dynamic Fading Channel | 5 Users: 3 Weak + 2 Strong | Code rates=[1/6,1/3,1/2,2/3,2/3]');
title ("Throughput evolution in different system configurations");

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%