
#ifndef CNGE_GATHERSECTION
#define CNGE_GATHERSECTION

#include "../LoadSection.h"

namespace CNGE {
	class GatherSection : public LoadSection {
	private:
		bool wait();

		bool skip();

		void load();

	public:
		GatherSection(LoadSection*);

	};

}

#endif
