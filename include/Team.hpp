#ifndef TEAM_HPP
#define TEAM_HPP

#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>

const int MAX_PROBLEMS = 26;

struct Team {
    int id;  // 0-based
    std::string name;

    struct Problem {
        bool solved = false;
        int firct_ac_time = 0;
        int failed_attempts = 0; // 封榜前
        int frozen_fails = 0;  // 封榜后
        int frozen_ac_time = 0;
    } problems[MAX_PROBLEMS];

    int total_solved = 0;
    int total_penalty = 0;
    std::vector<int> ac_times;

    bool has_frozen = false;
    std::priority_queue<int> frozen_problems;

    Team(int id, std::string& name);

    void update_problem(int problem_id, int time, bool solved, bool frozen);
    void unfreeze_problem();

    bool operator<(const Team& o) const;
};

class TeamManager {
private:
    std::vector<Team> teams;
    std::unordered_map<std::string, int> name2id;
    std::vector<std::string> id2name;
    int cnt_teams = 0;

public:
    bool add_team(const std::string& name);
    Team& get_team(int id);
    Team& get_team(const std::string& name);
    int get_team_count() const;
    const std::vector<Team>& get_all_teams() const;
};

#endif // TEAM_HPP