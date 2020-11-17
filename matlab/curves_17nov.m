clear all; close all; clc;


P_rx = -120:2:-80;

xlim([-120 -80])


BER_MSG = [0.0928178267045 0.0412093874008 0.0362413194444 0.015988817402 0.0221741691468 0.00404357910156];
BER_CW = [0.203702059659 0.0508432539683 0.0769675925926 0.0318148743873 0.0406203497024 0.00849151611328];


figure(1)
yyaxis left
semilogy(snr, BER_CW,'-ob','LineWidth',2);
grid on; hold on;
semilogy(snr, BER_MSG,'-or','LineWidth',2);
hleg1 = legend('Uncoded','Coded');
xlabel('RX GAIN');
ylabel('BER');
% 
% PKT_LOSS =  [59 58 59 58 59 60 61 64]
% TOTAL_PKT = [73 70 68 68 69 69 70 69]
% yyaxis right
% plot(snr, 1-PKT_LOSS./TOTAL_PKT,'--k');
% hleg1 = legend('Uncoded','Coded','Packet Loss');
% ylabel('Packet Loss Rate')

title('BER - CorteXlab Test (TX Gain=0)')
% figure(1)
% semilogy(snr, max(eps, BER_CW),'-ob','LineWidth',2);
% grid on; hold on;
% semilogy(snr, max(eps, BER_MSG),'-or','LineWidth',2);

