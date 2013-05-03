#include "utils_web.h"
#include "tinyxml/tinyxml.h"
#include <cxxtools/loginit.h>

log_define("sinfonifry.web.utils")

using namespace  std;

namespace  sinfonifry
{

std::string fix_entrypoints(const std::string &s, const string &plugin)
{
    string result;
    int i=0;

    while(i<s.length())
    {
        // is this an entrypoint tag? put here a nicely formatted <a href> tag
        if(s[i] == '<' && i <s.length() - 3 && s[i+1] == '@' && s[i+2] == 'e' && s[i+3] == 'p')
        {
            result += "<a href=\"/ep_manager?plugin=";
            result += plugin;

            i += 4; // skip the @ep

            while(isspace(s[i])) i++; // skip the spaces

            std::string newTag = "<ep ";
            while(s[i] != '>' && i < s.length())
            {
                newTag += s[i];
                i++;
            }
            if( i<s.length() )
            {
                newTag += "/"; // to close it
                newTag += s[i];
            }
            else
            {
                log_error("Malformed ep call: " << newTag);
                return s;
            }

            i++; // skip the '>'

            log_debug(newTag);

            std::string ep_name;
            std::string ep_params;
            std::string ep_string_to_display;

            TiXmlDocument doc;
            bool success = doc.FromMemory(newTag.c_str());
            if(success)
            {
                TiXmlElement* el_for_ep = doc.FirstChildElement("ep");
                if(el_for_ep)
                {
                    const char* s_ep_name = el_for_ep->Attribute("name");
                    if(s_ep_name) ep_name = s_ep_name;

                    const char* s_ep_params = el_for_ep->Attribute("params");
                    if(s_ep_params) ep_params = s_ep_params;

                    const char* s_ep_display = el_for_ep->Attribute("display");
                    if(s_ep_display) ep_string_to_display = s_ep_display;
                }
            }

            if(!ep_name.empty())
            {
                result += "&ep=";
                result += ep_name;
            }

            if(!ep_params.empty())
            {
                result += "&params=1&";
                result += ep_params;
            }
            else
            {
                result += "&params=0";
            }

            result += "&sinfonifry_target=div_";
            result += plugin;

            result +="\">";
            if(!ep_string_to_display.empty())
            {
                result += ep_string_to_display;
            }

            result += "</a>";
        }
        if(s[i] == '<' && i <s.length() - 3 && s[i+1] == 'a' && s[i+2] == ' ') // is this an "a" tag?. If yes append the target_div
        {
            i+=3; // skip "<a "
            result += "<a ";
            while(i < s.length() && s[i] != '>') // searching the closing tag
            {
                if(s[i] == 'h' && i <s.length() - 4 && s[i+1] == 'r' && s[i+2] == 'e' && s[i+3] == 'f')
                {
                    // found the href attribute, patch it.
                    i += 4; // skipping the "href" text
                    result += "href";
                    while(s[i] != '"') // adding the = sign and what might be there
                    {
                        result += s[i];
                        i++;
                    }

                    result += s[i]; // adding the starting double quote
                    i ++;

                    while(s[i] != '"')  // adding the rest
                    {
                        result += s[i];
                        i++;
                    }

                    // patching it
                    result += "&sinfonifry_target=div_";
                    result += plugin;

                    result += s[i]; // adding the ending double quote
                    i ++;

                    // now this should be fine .. hopefully

                }
                else
                {
                    result += s[i];
                    i++;
                }
            }
        }
        else
        {
            result += s[i];
            i++;
        }
    }

    log_info(result);

    return result;
}

}
