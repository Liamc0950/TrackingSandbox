#include <vector>

#include <cue.h>

class CueList
{
  private:
	std::vector<Cue> *cues;

  public:
	CueList(int number_of_cues);

    int get_cue_count();
    void add_cue(Cue cue);
    void process_cues();
    std::vector<Cue> get_cues();

};