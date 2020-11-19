clear all; close all; clc;


%user_0_ber = [0 0 0 0.00335317460317 0.0843253968254 0.0829365079365 NaN NaN NaN];
%user_1_ber = [0 0 0 0 0 0 0.00428571428571 0.0906746031746 0.135337301587];
%user_2_ber = [0 0 0 0 0 0 0.0170634920635 0.166666666667 NaN];
%user_3_ber = [0 0 0 0.00448412698413 0.056369047619 0.125396825397 0.795277777778 0.983952381  NaN];
%user_4_ber = [0 0 0 0 0 0 0.00176785714286 0.0860178571429 0.150357142857];


%user_0_thr = [];
%user_1_thr = [26.6807834833 26.6807834833 26.6807834833 24.938149431 24.938149431 24.938149431 24.9052109181 22.415245737 5.227678571 ];
%user_2_thr = [];
%user_3_thr = [];
%user_4_thr = [32.5392213829 32.5392213829 32.5392213829 32.5392213829 27.4319643741 27.4319643741 26.8348214286 21.8078397955 7.212301587];



P_tx = 32:-4:0

user_1_ber = [0 0 0 0 0 0 0.00428571428571 0.0906746031746 0.135337301587];
user_4_ber = [0 0 0 0 0 0 0.00176785714286 0.0860178571429 0.150357142857];

user_1_thr = [26.6807834833 26.6807834833 26.6807834833 24.938149431 24.938149431 24.938149431 24.9052109181 22.415245737 5.227678571 ];
user_4_thr = [32.5392213829 32.5392213829 32.5392213829 32.5392213829 27.4319643741 27.4319643741 26.8348214286 21.8078397955 7.212301587];

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
title (hax(2), 'BER vs Throughput | 5 Users | 4 Weak + 1 Strong | Rw = 1/3 | Rs = 2/3');



%title('BER vs Throughput | 5 Users | 4 Weak + 1 Strong | R_W = 1/3 | R_S = 1/2 ')

%user_0_ber = [0 0 0 0.00335317460317 0.0843253968254 0.0829365079365 NaN NaN NaN];
%user_1_ber = [0 0 0 0 0 0 0.00428571428571 0.0906746031746 0.135337301587];
%user_2_ber = [0 0 0 0 0 0 0.0170634920635 0.166666666667 NaN];
%user_3_ber = [0 0 0 0.00448412698413 0.056369047619 0.125396825397 0.795277777778 0.983952381  NaN];
%user_4_ber = [0 0 0 0 0 0 0.00176785714286 0.0860178571429 0.150357142857];
%
%
%figure(2)
%semilogy(P_tx, user_0_ber,'-o','LineWidth',2);
%grid on; hold on;
%semilogy(P_tx, user_1_ber,'-o','LineWidth',2);
%semilogy(P_tx, user_2_ber,'-o','LineWidth',2);
%semilogy(P_tx, user_3_ber,'-o','LineWidth',2);
%semilogy(P_tx, user_4_ber,'-o','LineWidth',2);
%hleg1 = legend('User 0 | R=','Weak User 1','Weak User 2','Weak User 3','Strong User 4');
%xlabel('TX GAIN');
%ylabel('BER');
%title('BER | 5 Users')



%P_rx = -120:2:-80;
%
%xlim([-120 -80])
%
%
%BER_MSG = [0.0928178267045 0.0412093874008 0.0362413194444 0.015988817402 0.0221741691468 0.00404357910156];
%BER_CW = [0.203702059659 0.0508432539683 0.0769675925926 0.0318148743873 0.0406203497024 0.00849151611328];
%
%
%figure(1)
%yyaxis left
%semilogy(snr, BER_CW,'-ob','LineWidth',2);
%grid on; hold on;
%semilogy(snr, BER_MSG,'-or','LineWidth',2);
%hleg1 = legend('Uncoded','Coded');
%xlabel('RX GAIN');
%ylabel('BER');


% 
% PKT_LOSS =  [59 58 59 58 59 60 61 64]
% TOTAL_PKT = [73 70 68 68 69 69 70 69]
% yyaxis right
% plot(snr, 1-PKT_LOSS./TOTAL_PKT,'--k');
% hleg1 = legend('Uncoded','Coded','Packet Loss');
% ylabel('Packet Loss Rate')

%title('BER - CorteXlab Test (TX Gain=0)')
% figure(1)
% semilogy(snr, max(eps, BER_CW),'-ob','LineWidth',2);
% grid on; hold on;
% semilogy(snr, max(eps, BER_MSG),'-or','LineWidth',2);

