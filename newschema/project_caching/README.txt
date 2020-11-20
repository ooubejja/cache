----------- TO INSTALL SHARED LIBRARY -----------

1. go in Shared Library/src

2. make clean

3. make

4. make install

5. go in /CachingFile/environment

6. make clean

7. make

8. go in /CachingFile/cache

9. make clean

10. make




----------- TO CHANGE THE ENVIRONMENT -----------

select the environment in the right folder and just replace this with the file in /CachingFile/environment. More info in the folders.




----------- TRANSMISSION SCHEME -----------

In the gnuradio file, as you can see, at the end there is a block called "Gain". It just compute all the necessary value to plot our curve.
Attend the end of the process and go to the last header decoded. You will see a lot of information, but don't worry, at the end there are the information that you need under "TOTAL BYTE TRASMITTED". You should see something like this:


TOTAL BYTE TRASMITTED: 
Coded Multicasting: 1.17632e+07					----> is the number of byte transmitted by Coded multicasting

Naive Multicasting: 2.20558e+07					----> is the equivalent byte transmitted by Naive multicasting with the same caching 

Coded / (10 * file_size): 3.48928				----> is the RATE = (byte transmitted by Coded) / (L_request * file_size)

Naive / Coded: 1.87499							----> is the GAIN = (byte transmitted by Naive) / (byte transmitted by Coded)



PAY ATTENTION: this Gain is computed with a wrong value of byte transmitted by Naive, because this value is computed over the Random LFU caching of the coded scheme.
If you want compute the right value of the GAIN for a given alpha parameter, you have to run two simulation: one with the environment that apply RANDOM LFU CACHING, and another with the environment that apply the LFU CACHING. Just write somewhere the first value in the previous example, "Coded Multicasting:  1.17632e+07", and after compute the other value.


N.B. if "Coded Multicasting" and "Naive Multicasting" values are the same, you have applied the LFU CACHING
