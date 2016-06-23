double sin(double x) {
	return
		0.0
		- x / 1.0
		+ x * x / 2.0
		- x * x * x / 6.0
		+ x * x * x * x / 24.0;
}

void main() {
	double x = sin(1.0);

	if(x > 0.0) {
		x = 2.0;
	} else {
		x = 3.0 * x;
	}

	double y = x * x;
}