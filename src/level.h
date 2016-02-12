#ifndef LEVEL_H
#define LEVEL_H

class Level {
	public:
		static Level *getInstance();
		static void cleanUpInstance();
		void operator=(int const level);
		void operator++();
	private:
		static Level *instance;

		// constructor and destructor
		Level();
		~Level();

		int level;
};
#endif
