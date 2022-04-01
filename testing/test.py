import chess.pgn

ENGINE_FILE = "../run.sh"
ALGO = "minmax"

pgn = open("p.pgn")
game = chess.pgn.read_game(pgn)
print(game)
engine = chess.engine.SimpleEngine.popen_uci([ENGINE_FILE, ALGO])
total = 0
correct = 0
while total != 100 and game:
    total += 1
    result = engine.play(game.board(), chess.engine.Limit(30))
    if list(game.mainline_moves())[0].uci() == result.move.uci():
        correct += 1
    else: print("fen:", game.headers["FEN"], list(game.mainline_moves())[0].uci(), result.move.uci())
    game = chess.pgn.read_game(pgn)
print(correct/total*100)
engine.quit()

