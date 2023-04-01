#include "DBHelper.h"
#include "Leaderboard.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

const std::string DATABASE_NAME = "oguns_dice.db";

using boost::shared_ptr;
DBHelper sql;

class LeaderboardHandler : virtual public LeaderboardIf {
public:
	LeaderboardHandler() {
		// Your initialization goes here
		this->highScoreByPlayer = new std::map<int32_t, int32_t>();

		// Client database
		sql.Connect(DATABASE_NAME.c_str());
		
	}

	void setHighScore(const int32_t playerId, const int32_t highScore) {
		this->highScoreByPlayer->insert(std::pair<int32_t, int32_t>(playerId, highScore));

		// Set high score
		sql.SetHighScore(playerId, highScore);
	}

	void getTop20(std::map<int32_t, int32_t> & _return) {
		// Get Top High scores
		std::map<int32_t, int32_t> result = sql.GetTopHighScores();

		_return.insert(result.begin(), result.end());
	}
private:
	std::map<int32_t, int32_t> *highScoreByPlayer;
};

int main(int argc, char **argv) {
	WSAData wsaData;
	int initializationResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (initializationResult != 0) {
		printf("Failed to init winsock %d\n", initializationResult);
		return 1;
	}

	int port = 9090;
	shared_ptr<LeaderboardHandler> handler(new LeaderboardHandler());
	shared_ptr<TProcessor> processor(new LeaderboardProcessor(handler));
	shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
	shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
	shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

	TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
	server.serve();
	return 0;
}

