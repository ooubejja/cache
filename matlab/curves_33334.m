clear all; close all; clc;

P_tx = 32:-4:0

user_1_ber = [0.00E+00 0.00E+00 0.00E+00 0.00E+00 5.47E-03 6.50E-03 6.73E-03 3.12E-02 5.75E-02];
user_4_ber = [0.00E+00 0.00E+00 3.90E-04 8.11E-04 2.12E-03 2.26E-03 3.81E-03  3.26E-02 1.12E-01];

user_1_thr = [3.92E+01 3.92E+01 3.92E+01 3.92E+01 3.90E+01 3.90E+01 3.90E+01 3.70E+01 3.30E+01];
user_4_thr = [4.34E+01 4.32E+01 4.30E+01 4.33E+01 4.33E+01 4.32E+01 4.34E+01 4.07E+01 3.64E+01];

figure(1)
grid on; hold on;
color = get (gca, "ColorOrder");

[hax, a1, b1] = plotyy (P_tx, user_1_ber, P_tx, user_1_thr,@semilogy,@plot);
[~, a4, b4] = plotyy (P_tx, user_4_ber, P_tx, user_4_thr,@semilogy,@plot);


% Color Style by User
set ([a1, b1], "marker", "o", 'LineWidth',2, "color", color(1,:));
set ([a4, b4], "marker", "o", 'LineWidth',2, "color", color(4,:));

% Line Style by metric
set ([a1, a4], "linestyle", "-");
set ([b1, b4], "linestyle", "-.");

set(hax,{'ycolor'},{'k';'k'})
grid on;
xlabel (hax(1), "xlabel");
ylabel (hax(1), "BER");
ylabel (hax(2), "Throughput (bits/channel use)");

xlabel('TX GAIN');
hleg1 = legend('Weak User BER','Strong User BER','Weak User Throughput','Strong User Throughput',"location", "southeast");
%title (hax(2), 'BER vs Throughput | 5 Users | 3 Weak + 2 Strong | Rw = 1/4 | Rs = 1/2');
title (hax(2), 'Blind BER vs Throughput | 5 Users | 4 Weak + 1 Strong | CodeRate=(3,3,3,3,4)');

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%