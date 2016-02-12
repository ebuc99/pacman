#ifndef LEVEL_H
#define LEVEL_H

class Level {
	public:
		static Level *getInstance();
		static void cleanUpInstance();
	private:
		static Level *instance;

		// constructor and destructor
		Level();
		~Level();
}
#endif
