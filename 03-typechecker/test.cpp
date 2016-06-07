string to_string(int i) {
	return "a number";
}

int main(int argc, string argv) {
	int x = 1;
	{
		double x = 2.0;
		{
			string x = to_string(3);
			{
				bool x = true;
				{
					{
						if(x)
						{
							x && true;
						} else
						{
							bool y = false;
							x = x || y;
						}
					}
				}
			}
		}
	}
}
