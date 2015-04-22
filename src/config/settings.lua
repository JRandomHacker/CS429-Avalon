-- num_players

all_players_per_quest = { { 1, 1, 1, 1, 1 },
                          { 1, 2, 1, 2, 2 },
                          { 1, 2, 2, 3, 2 },
                          { 2, 2, 3, 2, 3 },
                          { 2, 3, 2, 3, 3 },
                          { 2, 3, 4, 3, 4 },
                          { 2, 3, 3, 4, 4 },
                          { 3, 4, 4, 5, 5 },
                          { 3, 4, 4, 5, 5 },
                          { 3, 4, 4, 5, 5 } }

all_fails_per_quest = { { 1, 1, 1, 1, 1 },
                        { 1, 1, 1, 1, 1 },
                        { 1, 1, 1, 1, 1 },
                        { 1, 1, 1, 1, 1 },
                        { 1, 1, 1, 1, 1 },
                        { 1, 1, 1, 1, 1 },
                        { 1, 1, 1, 2, 1 },
                        { 1, 1, 1, 2, 1 },
                        { 1, 1, 1, 2, 1 },
                        { 1, 1, 1, 2, 1 } }

players_per_quest = all_players_per_quest[num_players]
fails_per_quest = all_fails_per_quest[num_players]
quest_track_length = 5
vote_track_length = 5
