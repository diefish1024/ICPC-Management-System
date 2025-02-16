#include <iostream>
#include <string>
#include <sstream>
#include "RankTree.hpp"
#include "Team.hpp"
#include "InputBuffer.hpp"

int main() {
    freopen("my.in", "r", stdin);
    freopen("my.out", "w", stdout);

    RankTree rank_tree;
    TeamManager team_manager;
    InputBuffer input_buffer;

    int duration_time = 0;

    // bufore START
    while (true) {
        input_buffer.readInput();
        const std::string& buffer = input_buffer.getBuffer();
        std::istringstream iss(buffer);
        std::string token;
        iss >> token;
        if (token == "ADDTEAM") {
        // if (token[0] == 'A') {
            // ADDTEAM [team_name]
            std::string name;
            iss >> name;
            if (!team_manager.add_team(name)) {
                std::cout << "[Error]Add failed: duplicated team name.\n";
            } else {
                // DEBUG
                // team_manager.last_team().print_team();

                rank_tree.insert(new RBNode(&team_manager.last_team()));
                std::cout << "[Info]Add successfully.\n";
            }
        }
        // if (token == "START") {
        if (token[0] == 'S') {
            // START DURATION [duration_time] PROBLEM [problem_count]
            iss >> token >> duration_time >> token >> Team::problem_count;

            // DEBUG
            // std::cout << "duration_time: " << duration_time << std::endl;
            // std::cout << "problem_count: " << Team::problem_count << std::endl;

            std::cout << "[Info]Competition starts.\n";
            break;
        }

        // DEBUFG
        if (token == "p1") {
            rank_tree.print_tree();
        }
        if (token == "p2") {
            team_manager.print_all_teams();
        }
    }

    // rank_tree.print_tree();

    // during COMPETITION
    bool is_frozen = false;
    while(true) {
        input_buffer.readInput();
        const std::string& buffer = input_buffer.getBuffer();
        std::istringstream iss(buffer);
        std::string token;
        iss >> token;
        if (token == "START") {
            std::cout << "[Error]Start failed: competition has started.\n";
        }
        if (token == "SUBMIT") {
        // if (token[0] == 'S') {
            // SUBMIT [problem_name] BY [team_name] WITH [submit_status] AT [time]
            char problem_name;
            std::string team_name;
            int problem_id, time;
            std::string result;
            iss >> problem_name >> token >> team_name >> token >> result >> token >> time;
            problem_id = problem_name - 'A';

            Team& team = team_manager.get_team(team_name);

            RBNode* node = rank_tree.find(&team);
            rank_tree.remove(node);

            team.update_problem(problem_id, time, result == "Accepted", is_frozen);
            rank_tree.insert(new RBNode(&team));

            // DEBUG
            // team.print_team();
            // rank_tree.print_tree();
        }
        if (token == "END") {
        // if (token[0] == 'E') {
            // END
            std::cout << "[Info]Competition ends.\n";
            break;
        }

        // DEBUFG
        if (token == "p1") {
            rank_tree.print_tree();
        }
        if (token == "p2") {
            team_manager.print_all_teams();
        }
    }

    return 0;
}