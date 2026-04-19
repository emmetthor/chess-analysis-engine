#include "Engine.h"
#include "move/Move.h"
#include <string>
#include <sstream>

static Engine g_engine;
static std::string g_lastJson;

static std::string escapeJson(const std::string& s)
{
    std::string out;
    for (char ch : s)
    {
        if (ch == '\\') out += "\\\\";
        else if (ch == '"') out += "\\\"";
        else out += ch;
    }
    return out;
}

extern "C" {

int web_set_fen(const char* fen)
{
    if (fen == nullptr)
        return 0;

    try
    {
        g_engine.setPositionWithFen(std::string(fen));
        return 1;
    }
    catch (...)
    {
        return 0;
    }
}

const char* web_analyze_depth(int depth)
{
    std::cout << "web analyze depth starts" << std::endl;

    try
    {
        std::cout << "depth: " << depth << std::endl;

        g_engine.ping();

        std::cout << "ping pong !!" << std::endl;

        SearchResult res = g_engine.fullInfoSearch(depth);

        std::string bestMove = moveToUCIMove(res.bestMove);

        // 如果你還沒有 PV，就先用 bestMove 代替
        std::string pv = bestMove;

        std::ostringstream oss;
        oss << "{"
            << "\"ok\":true,"
            << "\"depth\":" << depth << ","
            << "\"bestMove\":\"" << escapeJson(bestMove) << "\","
            << "\"pv\":\"" << escapeJson(pv) << "\"";

        // 如果你的 SearchResult 有 score/nodes/time，就一起塞
        oss << ",\"scoreCp\":" << res.bestScore;
        // oss << ",\"nodes\":" << res.nodes;
        // oss << ",\"timeMs\":" << res.timeMs;

        oss << "}";

        g_lastJson = oss.str();
        return g_lastJson.c_str();
    }
    catch (...)
    {
        g_lastJson = "{\"ok\":false}";
        return g_lastJson.c_str();
    }
}

void web_set_startpos()
{
    g_engine.setStartPosition();
}

}