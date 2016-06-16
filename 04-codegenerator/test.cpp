double sin(double x) {
	return 
		0.0
		- x / 1.0
		+ x * x / 2.0 
		- x * x * x / 6.0 
		+ x * x * x * x / 24.0;
}

double main(int argc, int argv) {
	bool   debug = true;
	int    i     = 0;
	double pi    = 3.14159265358979323846264;
	return sin(pi);
}