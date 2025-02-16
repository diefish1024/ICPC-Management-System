#include "Team.hpp"
#include <algorithm>

int Team::problem_count = 0;

Team::Team(int id, std::string& name) : id(id), name(name) {
    ac_times.reserve(MAX_PROBLEMS);
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
    for (int i = total_solved - 1; i >= 0; --i) {
        if (ac_times[i] != o.ac_times[i]) return ac_times[i] < o.ac_times[i];
    }
    return name < o.name;
}

void Team::print_team() const {
    std::cout << "Team: " << name << std::endl;
    std::cout << "Total solved: " << total_solved << std::endl;
    std::cout << "Total penalty: " << total_penalty << std::endl;
    std::cout << "AC times: ";
    for (int t : ac_times) {
        std::cout << t << " ";
    }
    std::cout << std::endl;
    std::cout << "Problems:" << std::endl;
    for (int i = 0; i < problem_count; ++i) {
        if (problems[i].solved) {
            std::cout << "Problem " << i << ": solved at " << problems[i].firct_ac_time << std::endl;
        } else {
            std::cout << "Problem " << i << ": not solved" << std::endl;
        }
    }
    std::cout << std::endl;
}

TeamManager::TeamManager() {
    teams.reserve(MAX_TEAM);
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

Team& TeamManager::last_team() {
    return teams[cnt_teams - 1];
}

void TeamManager::print_all_teams() const {
    // std::vector<Team> teams_copy = teams;
    // std::sort(teams_copy.begin(), teams_copy.end());
    puts("Teams:");
    for (const auto& team : teams) {
        std::cout << team.id << " " << team.name << std::endl;
    }
}