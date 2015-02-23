struct RGB
{
public:
  int R;
  int G;
  int B;
};


class ColorSensor
{
public:
  RGB GetRGB();
  ColorSensor(int vcc, int out, int s2, int s3, int s0, int s1);
private:
  int red;
  int grn;
  int blu;

  int VCC;
  int OUT;
  int S2;
  int S3;
  int S0;
  int S1;
  
  int RGBFilter;
  long count;
  int counter;
  int G_flag ;
  float scaleFactor[3];

  void Select_Filters(int RGB);  
};

