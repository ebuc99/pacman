#ifndef RAIL_H
#define RAIL_H
class Rail {
	public:
		Rail(int init_x1, int init_y1, int init_x2, int init_y2);
		~Rail();
		int x1;
		int y1;
		int x2;
		int y2;
};
#endif