// Original serial code snippet retrieved from:
// https://rosettacode.org/wiki/Fast_Fourier_transform#C

#include <stdio.h>
#include <math.h>
#include <complex.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "recursive-fft.h"

// Get next highest power of 2. Adapted from
// https://stackoverflow.com/questions/466204/rounding-up-to-next-power-of-2
unsigned long next_2_power(unsigned long v) {
  v--;
  v |= v >> 1;
  v |= v >> 2;
  v |= v >> 4;
  v |= v >> 8;
  v |= v >> 16;
  v |= v >> 32;
  v++;
  return v;
}

int main(int argc, char *argv[])
{
	double PI = atan2(1, 1) * 4;
	// Defaults for duration and frequency can be overwritten by command line
	// arguments.
	double duration = 0.5;
	int freq = 60;
	unsigned int sampling_frequency = 44100;
  unsigned long num_samples;
  // Generate a test array based on command line input parameters. Make an array
  // at least as big as duration times sampling_frequency.
	if (argc == 3) {
		duration = atof(argv[2]);
	}
  if (argc > 3) {
		duration = atof(argv[2]);
		freq = atoi(argv[3]);
	}
  // Size of buf must be a power of 2 for algorithm to work.
	num_samples = next_2_power((unsigned long)(duration * sampling_frequency));
  // Initialize the buffer
	double complex* buf = (double complex*) malloc(num_samples * sizeof(double complex));
	memset(buf,(double complex)0,num_samples*sizeof(buf));

  // Fill the buffer with a num_samples number of samples spaced a step apart.
	// Adapted from
  // https://stackoverflow.com/questions/203890/creating-sine-or-square-wave-in-c-sharp
	double step = 0.0;
	// Write test wave input to file for gnuplot to render
	FILE *wavePlotFile = NULL;
	wavePlotFile = fopen("wave.txt","w");
	// Default to sine wave
	if (!argv[1] || strcmp(argv[1],"-sine") == 0) {
    for (int i = 0; i < num_samples; i++) {
			step += duration / num_samples;
			buf[i] = (double complex)(sin(2 * PI * step * freq));
			fprintf(wavePlotFile, "%f\t%g\n",step,creal(buf[i]));
    }
	}
	else if (!argv[1] || strcmp(argv[1],"-octaves") == 0) {
    for (int i = 0; i < num_samples; i++) {
			step += duration / num_samples;
			buf[i] = (double complex)(sin(2 * PI * step * freq)) +
				0.33 * (double complex)(sin(2 * PI * step * 2 * freq)) +
				0.11 * (double complex)(sin(2 * PI * step * 3 * freq));
			fprintf(wavePlotFile, "%f\t%g\n",step,creal(buf[i]));
    }
	}
	// Generate noise by using random values
	else if (strcmp(argv[1],"-noise") == 0) {
		int r = 0;
		double randD = 0.0;
		for (int i = 0; i < num_samples; i++) {
			step += duration / num_samples;
			r = rand();
			randD = (double)r/INT_MAX;
			buf[i] = (double complex)(randD + (r % 3) - 1);
			fprintf(wavePlotFile, "%f\t%g\n",step,creal(buf[i]));
    }
	}
	// Generate a sine with two octaves with noise
	else if (!argv[1] || strcmp(argv[1],"-noisy") == 0) {
		int r = 0;
		double randD = 0.0;
    for (int i = 0; i < num_samples; i++) {
			step += duration / num_samples;
			r = rand();
			randD = (double)r/INT_MAX;
			buf[i] = (double complex)(sin(2 * PI * step * freq)) +
				0.33 * (double complex)(sin(2 * PI * step * 2 * freq)) +
				0.11 * (double complex)(sin(2 * PI * step * 3 * freq)) +
				0.05 * (double complex)(randD + (r % 3) - 1);
			fprintf(wavePlotFile, "%f\t%g\n",step,creal(buf[i]));
    }
	}
	fclose(wavePlotFile);

	// Initialize the out buffer.
	double complex* out = (double complex*) malloc(num_samples * sizeof(double complex));
	memset(out,(double complex)0,num_samples*sizeof(out));
	for (int i = 0; i < num_samples; i++) {
		out[i] = buf[i];
	}
	// Run the FFT
	_fft(buf, out, num_samples, 1);

	// Plot the component frequencies represented by the FFT. Adapted from
	// Python code found here:
	// https://www.ritchievink.com/blog/2017/04/23/understanding-the-fourier-transform-by-example/
	FILE *fftPlotFile = NULL;
	fftPlotFile = fopen("fft.txt","w");
	double freq_step = 0.0;
	int end = num_samples / 2;
	double T = duration / num_samples;
	double complex val;
	for (int i = 0; i < end; i++) {
		val = (double complex)(fabs(buf[i]) / num_samples);
		fprintf(fftPlotFile, "%f\t%g\n",freq_step,creal(val));
		freq_step += (1/T) / num_samples;
	}
	fclose(fftPlotFile);
	free(buf);
	free(out);
	return 0;
}
