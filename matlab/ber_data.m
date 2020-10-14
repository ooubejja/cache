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
snr = 1:1:11;
BER_CW = [0.0990203373016  0.0675856370192 0.0434646606445 0.0285982572115  0.0157977764423 0.008203125  0.00355529785156 0.00155009920635 0.000419616699219 5.91856060606e-05 2.28881835938e-05];
BER_MSG = [0.0483010912698 0.0281475360577  0.00762348790323 0.00285456730769 0.000811298076923  0.000270432692308  0 0 0 0 0];

figure(1)
semilogy(snr, BER_CW,'-ob','LineWidth',2);
grid on; hold on;
semilogy(snr, BER_MSG,'-or','LineWidth',2);

hleg1 = legend('Uncoded','Coded');

xlabel('Non normalized SNR');
ylabel('BER');
% figure(1)
% semilogy(snr, max(eps, BER_CW),'-ob','LineWidth',2);
% grid on; hold on;
% semilogy(snr, max(eps, BER_MSG),'-or','LineWidth',2);

