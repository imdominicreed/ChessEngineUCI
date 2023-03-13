import chess.pgn

ENGINE_FILE = "/Users/domino/Documents/Projects/thi-bot/ChessEngineUCI/build/uci"
ALGO = "tpalphabeta"
pgn = open("testing/p.pgn")
game = chess.pgn.read_game(pgn)
engine = chess.engine.SimpleEngine.popen_uci([ENGINE_FILE, ALGO])
total = 0
correct = 0
while total != 100 and game:
    total += 1
    result = engine.play(game.board(), chess.engine.Limit(time=5))
    if list(game.mainline_moves())[0].uci() == result.move.uci():
        correct += 1
    else:
        print("fen:", game.headers["FEN"], list(
            game.mainline_moves())[0].uci(), result.move.uci())
    game = chess.pgn.read_game(pgn)
print(correct/total*100)
engine.quit()
