@echo off
python "%~dp0playgame.py" --engine_seed 42 --player_seed 42 --end_wait=0.25 --verbose --log_dir game_logs --turns 500 --map_file "maps/example/tutorial1.map" %* "python ""%~dp0sample_bots\python\HunterBot.py""" "submission_test\MyBot.exe"

