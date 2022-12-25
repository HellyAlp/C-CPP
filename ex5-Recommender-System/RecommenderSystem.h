#ifndef EX5_RECOMMENDERSYSTEM_H
#define EX5_RECOMMENDERSYSTEM_H

#include <string>
#include <unordered_map>
#include <vector>
using std::string;
using std::unordered_map;
using std::vector;
/**
 * A class representing the recommending system.
 */
class RecommenderSystem
{
public:
/**
     * A method that gets paths to 2 files- moviesAttributesFile, userRanksFile and loads its data.
     * @param moviesAttributesFilePath file containing data about a movie by different attributes.
     * @param userRanksFilePath file containing movies ranked by users.
     * @return 0 upon success and -1 upon failre.
     */
    int loadData(  string moviesAttributesFilePath,  string userRanksFilePath);

    /**
     * A method that gets a user name and find the user most recommended movie by content.
     * @param userName string
     * @return string, name of the movie or error.
     */
    string recommendByContent(string userName);

    /**
     * A method that predicts the match of a movie to a user
     * @param movieName string, name of the movie
     * @param userName string, name of the user
     * @param k int, number of the similar movies (ranked by user) to the movie given.
     * @return -1 upon failre, positive double represents the predicted ranking upon success.
     */
    double predictMovieScoreForUser(string movieName, string userName, int k);

    /**
     * A method that gets a user name and an int representing the number of the similar movies (ranked by user)
     * and returns the name of the movie recommended to the user by the collaborative filtering
     * method.
     * @param userName string, name of user.
     * @param k int, number of the similar movies (ranked by user) to the movie given.
     * @return string, name of the movie or error
     */
    string recommendByCF(string userName, int k);


private:

    /**
     * Class privet variables
     */
    unordered_map<string, vector<double >> _movieAttributesDic;
    unordered_map<string, vector<double>> _userMoviesRankDic;
    unordered_map<string, double> _norms;
    vector<string> _movies;

    /**
     * A helper method to the load data method. A method that reads the given movie attribute file and loads it to
     * an data structure by parsing its data;
     * @param path path to the file
     * @return 0 Upon success or -1 upon failure.
     */
    int _movieFileParsing( string path);

    /**
     * A helper method to the load data method. A method that reads the given user ranking file and loads it to an data
     * structure by parsing its data;
     * @param path path to the file
     * @return 0 Upon success or -1 upon failure.
     */
    int _usersFileParsing( string path);


    /**
     * A method thar calculates the norm to a given vector.
     * @param name movie/user name
     * @param rates vector
     * @return the norm result.
     */
    double& _calculateNorm(string name, vector<double> rates);

};



#endif //EX5_RECOMMENDERSYSTEM_H
