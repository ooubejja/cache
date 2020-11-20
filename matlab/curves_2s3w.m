clear all; close all; clc;

P_tx = 32:-4:0

user_1_ber = [0 0 0 0 0 0 0.00767857142857 0.0833531746032 0.156944444444];
user_4_ber = [0 0 0 0 0 0 0.004375 0.0614821428571 0.148178571429];

user_1_thr = [33.8937457969 33.8937457969 33.8937457969 33.8937457969 33.8937457969 33.8482202821 33.6561238223 16.8048204159 12.0889348501];
user_4_thr = [29.0005178664 29.0005178664 24.8498431197 24.7058823529 24.7207207207 24.972972973 24.5440251572 21.402461258 10.0122222222];

figure(1)
grid on; hold on;
color = get (gca, "ColorOrder");

[hax, a1, b1] = plotyy (P_tx, user_1_ber, P_tx, user_1_thr,@semilogy,@plot);
[~, a4, b4] = plotyy (P_tx, user_4_ber, P_tx, user_4_thr,@semilogy,@plot);


%set ([a1, b1], "marker", "o", 'LineWidth',2, "color", color(1,:));
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
title (hax(2), 'BER vs Throughput | 5 Users | 3 Weak + 2 Strong | Rw = 1/4 | Rs = 1/2');

