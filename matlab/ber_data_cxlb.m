clear all; close all; clc;

%% NO OFDM 
% snr = 1:1:12;
% BER_CW = [0.130554199219 0.104843139648 0.0792292668269 0.0574776785714  0.037483215332  0.0230422247024 0.0130874171402 0.00590515136719  0.00251770019531  0.00067138671875  0.000221946022727 3.81469726562e-05];
% BER_MSG = [0.110260009766 0.0144195556641 0.000991586538462 0 0 0 0 0 0 0 0 0];
% 
% figure(1)
% semilogy(snr, BER_CW,'-ob','LineWidth',2);
% grid on; hold on;
% semilogy(snr, BER_MSG,'-or','LineWidth',2);
% 
% hleg1 = legend('Uncoded','Coded');
% xlabel('SNR');
% ylabel('BER');

%% OFDM
% snr = 0:2:14;
% BER_CW = [0.0482 0.0196 0.0042 0.000627 0.000163 1.57e-05 0 0];
% BER_MSG = [0.0313 0.0122 0.0012 2.7e-7 0 0 0 0];

snr = 0:2:10;

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

