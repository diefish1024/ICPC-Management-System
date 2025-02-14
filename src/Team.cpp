#include "Team.hpp"
#include <algorithm>

Team::Team(int id, std::string& name) : id(id), name(name) {
    ac_times.resize(MAX_PROBLEMS);
}

void Team::update_problem(int problem_id, int time, bool solved, bool frozen) {
    auto& p = problems[problem_id];
    if (p.solved) return;
    if (frozen) {
        has_frozen = true;
        if (p.frozen_fails == 0 && p.frozen_ac_time == 0) {
            frozen_problems.push(problem_id);
        }
    }
    if (solved) {
        if (!frozen) {
            p.solved = true;
            p.firct_ac_time = time;
            total_solved++;
            total_penalty += time + p.failed_attempts * 20;
            ac_times.push_back(time);
            std::sort(ac_times.begin(), ac_times.end());
        } else {
            if (p.firct_ac_time == 0) {
                p.frozen_ac_time = time;
            }
        }
    } else {
        if (!frozen) {
            p.failed_attempts++;
        } else {
            p.frozen_fails++;
        }
    }
}

void Team::unfreeze_problem() {
    auto& p = problems[frozen_problems.top()];
    frozen_problems.pop();
    p.solved = true;
    p.firct_ac_time = p.frozen_ac_time;
    total_solved++;
    p.failed_attempts += p.frozen_fails;
    total_penalty += p.frozen_ac_time + p.failed_attempts * 20;
    ac_times.push_back(p.frozen_ac_time);
    std::sort(ac_times.begin(), ac_times.end());
}

bool Team::operator<(const Team& o) const {  // if a < b, a is better than b
    if (total_solved != o.total_solved) return total_solved > o.total_solved;
    if (total_penalty != o.total_penalty) return total_penalty < o.total_penalty;
    for (int i = total_solved - 1; i; --i) {
        if (ac_times[i] != o.ac_times[i]) return ac_times[i] < o.ac_times[i];
    }
    return name < o.name;
}

bool TeamManager::add_team(const std::string& name) {
    if (name2id.find(name) != name2id.end()) return false;
    name2id[name] = cnt_teams;
    id2name.push_back(name);
    teams.emplace_back(cnt_teams, id2name[cnt_teams]);
    cnt_teams++;
    return true;
}

Team& TeamManager::get_team(int id) {
    return teams[id];
}

Team& TeamManager::get_team(const std::string& name) {
    return teams[name2id[name]];
}

int TeamManager::get_team_count() const {
    return cnt_teams;
}

const std::vector<Team>& TeamManager::get_all_teams() const {
    return teams;
}