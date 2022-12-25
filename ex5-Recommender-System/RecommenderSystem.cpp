#include "RecommenderSystem.h"
#include <cmath>
#include <numeric>
#include <iostream>
#include <istream>
#include <sstream>
#include <fstream>
#include <functional>
#define SUCCESS 0
#define FILE_ERROR "Unable to open file "
#define USER_NOT_FOUND "USER NOT FOUND"
#define FAILURE -1
#define DOUBLE_ZERO 0.0
#define ONE 1
#define ZERO 0
using std::inner_product;
using std::sqrt;
using std::stable_sort;
using std::cerr;
using std::endl;
using std::numeric_limits;
using std::transform;


/**
 * A helper method to the load data method. A method that reads the given user ranking file and loads it to an data
 * structure by parsing its data;
 * @param path path to the file
 * @return 0 Upon success or -1 upon failure.
 */
int RecommenderSystem::_usersFileParsing(const string path)
{
    std::ifstream  file;
    file.open(path);
    string line, movies, userName;
    vector<double> movieAttribute, ranks;
    double token;
    if(!file.is_open())
    {
        return FAILURE;
    }
    int row = ZERO;
    vector<string> movieNames;
    while (file.good() && std::getline(file, line))
    {
        std::istringstream iss(line);

        vector<std::pair< string, double >> userAttribute;
        if(row == ZERO)
        {
            while (iss >> movies || !iss.eof())
            {
                movieNames.push_back(movies);
            }
            row ++;
            continue;
        }
        iss >> userName;
        while (iss >> token || !iss.eof())
        {
            if(iss.fail())
            {
                iss.clear();
                string temp;
                iss >> temp;
                token = DOUBLE_ZERO;
            }
            ranks.push_back(token);
        }
        userAttribute.reserve(movieNames.size());
        _userMoviesRankDic[userName] = ranks;
        _movies = movieNames;

        ranks.clear();
        userAttribute.clear();
    }
    file.close();
    return SUCCESS;
}


/**
 *A helper method to the load data method. A method that reads the given movie attribute file and loads it to an
 * data structure by parsing its data;
 * @param path path to the file
 * @return 0 Upon success or -1 upon failure.
 */
int RecommenderSystem::_movieFileParsing(const string path)
{
    std::ifstream file;
    file.open(path);
    string line, movieName;
    vector<double> movieAttribute;
    double token;
    if(!file.is_open() )
    {
        return FAILURE;
    }
    while (file.good() && std::getline(file, line))
    {
        std::istringstream iss(line);
        iss >> movieName;
        while (iss >> token || !iss.eof())
        {
            movieAttribute.push_back(token);
        }
        _movieAttributesDic[movieName] = movieAttribute;
        movieAttribute.clear();
    }
    file.close();
    return SUCCESS;
}



/**
 * A method that gets paths to 2 files- moviesAttributesFile, userRanksFile and loads its data.
 * @param moviesAttributesFilePath file containing data about a movie by different attributes.
 * @param userRanksFilePath file containing movies ranked by users.
 * @return 0 upon success and -1 upon failre.
 */
int RecommenderSystem::loadData(  string moviesAttributesFilePath,   string userRanksFilePath)
{
    int firstFileRes = _movieFileParsing(moviesAttributesFilePath);
    int secondFileRes = _usersFileParsing(userRanksFilePath);
    if(firstFileRes == FAILURE)
    {
        string path = moviesAttributesFilePath;
        cerr << FILE_ERROR << path << endl;
        return FAILURE;
    }
    else if(secondFileRes == FAILURE)
    {
        string path = userRanksFilePath;
        cerr << FILE_ERROR << path << endl;
        return FAILURE;
    }
    return SUCCESS;

}

/**
 * A method that gets a user name and find the user most recommended movie by content.
 * @param userName string
 * @return string, name of the movie or error.
 */
string RecommenderSystem::recommendByContent(string userName)
{
    if(_userMoviesRankDic.find(userName) == _userMoviesRankDic.end())
    {
        return USER_NOT_FOUND;
    }
    vector<double> normalizedUserRanks = _userMoviesRankDic[userName];
    vector<double> userPref(normalizedUserRanks.size(), DOUBLE_ZERO);
    double  sum = DOUBLE_ZERO, count = DOUBLE_ZERO, avg = DOUBLE_ZERO;
    // Stage 1:
    for (double i : normalizedUserRanks)
    {
        if(i != ZERO)
        {
            sum += i;
            count++;
        }
    }
    avg = sum / count;
    for (double k = 0; k < normalizedUserRanks.size(); ++k)
    {
        if(normalizedUserRanks[k] != DOUBLE_ZERO)
        {
            normalizedUserRanks[k] = normalizedUserRanks[k] - avg;
        }
    }
    // Stage 2:
    vector<double> tempVec;
    for (double j = 0; j < normalizedUserRanks.size(); ++j)
    {
        tempVec = this->_movieAttributesDic[_movies[j]];
        if(normalizedUserRanks[j] != ZERO)
        {
            transform(tempVec.begin(), tempVec.end(), tempVec.begin(),
                    std::bind1st(std::multiplies<double >(), normalizedUserRanks[j]));
            transform(tempVec.begin(), tempVec.end(), userPref.begin(), userPref.begin(), std::plus<double >());
        }
        tempVec.clear();
    }
    string bestMovie;
    double bestMovieResult = -numeric_limits<double >::max();
    // Stage 3:
    for (double j = 0; j < normalizedUserRanks.size(); ++j)
    {
        tempVec = this->_movieAttributesDic[_movies[j]];
        double temp = DOUBLE_ZERO , result = DOUBLE_ZERO, scalar = DOUBLE_ZERO;
        if(_userMoviesRankDic[userName][j] == ZERO)
        {
            scalar = inner_product(tempVec.begin(), tempVec.end(), userPref.begin(), temp);
            result = scalar / ((_calculateNorm(userName, userPref)) * (_calculateNorm(_movies[j], tempVec)));
            if( result > bestMovieResult)
            {
                bestMovieResult = result;
                bestMovie = _movies[j];
            }
        }
        tempVec.clear();
    }
    return bestMovie;
}

/**
 * A method that predicts the match of a movie to a user
 * @param movieName string, name of the movie
 * @param userName string, name of the user
 * @param k int, number of the similar movies (ranked by user) to the movie given.
 * @return -1 upon failre, positive double represents the predicted ranking upon success.
 */
double RecommenderSystem::predictMovieScoreForUser(string movieName, string userName, int k)
{
    if(_userMoviesRankDic.find(userName) == _userMoviesRankDic.end() ||
       _movieAttributesDic.find(movieName) == _movieAttributesDic.end())
    {
        return FAILURE;
    }
    vector<double> movieRank = _movieAttributesDic[movieName];
    vector<double> userAtt = _userMoviesRankDic[userName];
    vector<double> seenMovieVector;
    vector<std::pair<double, string >> movieSimilarityVec;
    double prediction = DOUBLE_ZERO, upSum = DOUBLE_ZERO, downSum = DOUBLE_ZERO;
    //Stage 1:
    for (double j = 0; j < userAtt.size(); ++j)
    {
        seenMovieVector = this->_movieAttributesDic[_movies[j]];
        double scalar;
        double result = DOUBLE_ZERO;
        if(userAtt[j] != ZERO)
        {
            scalar = inner_product(seenMovieVector.begin(), seenMovieVector.end(), movieRank.begin(), DOUBLE_ZERO);
            result = scalar / ((_calculateNorm(_movies[j], seenMovieVector)) *
                               (_calculateNorm(movieName, movieRank)));
            movieSimilarityVec.emplace_back( result, _movies[j]);
        }
    }
    stable_sort(movieSimilarityVec.begin(), movieSimilarityVec.end());
    // Stage 2:
    for (int i = 0; i < k; ++i)
    {
        vector<string>::iterator iter = std::find(_movies.begin(), _movies.end(),
                movieSimilarityVec[movieSimilarityVec.size() -ONE - i].second);
        int index = std::distance(_movies.begin(), iter);
        upSum += movieSimilarityVec[movieSimilarityVec.size() -ONE - i].first * userAtt[index];
        downSum += movieSimilarityVec[movieSimilarityVec.size() -ONE - i].first;
    }
    prediction = upSum / downSum;
    return prediction;
}

/**
 * A method that gets a user name and an int representing the number of the similar movies (ranked by user)
 * and returns the name of the movie recommended to the user by the collaborative filtering
 * method.
 * @param userName string, name of user.
 * @param k int, number of the similar movies (ranked by user) to the movie given.
 * @return string, name of the movie or error
 */
string RecommenderSystem::recommendByCF(string userName, int k)
{
    if(_userMoviesRankDic.find(userName) == _userMoviesRankDic.end())
    {
        return USER_NOT_FOUND;
    }
    vector<double> userAtt = _userMoviesRankDic[userName];
    string bestMovie;
    double bestMoviePrediction = DOUBLE_ZERO;
    for (double i = 0; i < userAtt.size(); ++i)
    {
        if(userAtt[i] == ZERO )
        {
            double  prediction = predictMovieScoreForUser(_movies[i], userName, k);
            if(prediction > bestMoviePrediction)
            {
                bestMoviePrediction = prediction;
                bestMovie = _movies[i];
            }
        }
    }
    return bestMovie;
}

/**
 * A method thar calculates the norm to a given vector.
 * @param name movie/user name
 * @param rates vector
 * @return the norm result.
 */
double& RecommenderSystem::_calculateNorm(string movieName, vector<double> rates)
{
    if(_norms.find(movieName) == _norms.end())
    {
        double norm = DOUBLE_ZERO;
        double sum = DOUBLE_ZERO;
        for (double i : rates)
        {
            sum += std::pow(i, 2);
        }
        norm = sqrt(sum);
        _norms[movieName] = norm;
    }
    return _norms[movieName];
}




