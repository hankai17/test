#include <boost/regex.hpp>
#include <iostream>

int main() 
{
    boost::regex::flag_type flag;
    flag = boost::regex::perl;
    flag |= boost::regex::no_except;

    std::string regex, str;
    regex = "(\\d{3}[#-]){2}";

    const boost::regex e(regex, flag);
    if (e.status()) 
    {
        std::cout << "Incorrect regex pattern!\n";
        return 0;
    }
    str = "slfkasldf111-222-333-444-555-666-777-888-999-000-sladfkjlsad";
    boost::smatch result;
    bool matched = boost::regex_search(str, result, e); 
    std::cout << (matched ? "MATCH\n" : "DOES NOT MATCH\n");
    /*
    for(int i = 0; i < result.size(); i++)
    {
        std::cout<<"result[i]="<<result[i]<<std::endl;
    }
    */
    /*
    for(boost::smatch::iterator itr = result.begin(); itr != result.end(); itr++)
    {
        std::cout<<*itr<<std::endl;
    }
    */
    std::cout<<"matching str="<<result[0]<<std::endl;

    return 0;
} 

int main1() 
{
    boost::regex::flag_type flag;
    flag = boost::regex::perl;
    flag |= boost::regex::no_except;

    std::string regex, str;
    regex = "(\\d{3}[#-]){2}";

    const boost::regex e(regex, flag);
    if (e.status()) 
    {
        std::cout << "Incorrect regex pattern!\n";
        return 0;
    }
    str = "123-123-";
    bool matched = boost::regex_match(str, e); 
    std::cout << (matched ? "MATCH\n" : "DOES NOT MATCH\n");

    return 0;
} 
