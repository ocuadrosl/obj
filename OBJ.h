#ifndef OBJ_H
#define OBJ_H

#include <iostream>
#include <vector>
#include <array>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>


struct OBJ
{

    using faceType = std::array<std::array<unsigned,2>, 3>;

    std::string FileName = {};
    std::vector<glm::vec3> Coordinates;
    std::vector<glm::vec3> Normals;
    std::vector<faceType> Faces; //triangles

    OBJ(const std::string& fileName): FileName{fileName}
    {
        Read();
    };

    void GetOpenGLArrays(std::vector<glm::vec3>& glCoordinates, std::vector<glm::vec3>& glNormals);

private:
    void Read();

};

void OBJ::Read()
{

    char x{-127};
    std::cout<<x<<"\n";

    std::ifstream file{FileName};

    if(!file.is_open())
    {
        std::cerr<<"[ FAIL ] Open OBJ file\n";
        return;
    }

    std::string line;
    //float x,y,z;
    glm::vec3 vec3Aux;

    while(!file.eof())
    {
        std::getline(file, line);
        if(line.compare(0,2, "v ")==0) //empty space because it match with normals vn
        {
            std::stringstream values{line.erase(0,2)};

            values>>vec3Aux.x;
            values>>vec3Aux.y;
            values>>vec3Aux.z;
            Coordinates.emplace_back(vec3Aux);
            //std::cout<<vec3Aux.x<<"\n";

        }

        if (line.compare(0,2, "vn")==0)
        {
            std::stringstream values{line.erase(0,2)};
            values>>vec3Aux.x;
            values>>vec3Aux.y;
            values>>vec3Aux.z;
            Normals.emplace_back(vec3Aux);

            //std::cout<<vec3Aux.x<<"\n";

        }
        if(line.compare(0,1, "f")==0)
        {
            std::stringstream face{line.erase(0,2)};
            //std::cout<<"face :"<<face.str()<<"\n";
            std::string vertex, c,t,n;

            faceType faceAux;
            if(face.str().find('/') == std::string::npos) // not matches found
            {

                for(unsigned i=0; i<3; ++i)
                {
                    std::getline(face, c, ' ');
                    faceAux[i][0] =  std::stoul(c)-1;
                    //faceAux[i][0] =  static_cast<unsigned>(std::stoi(c)-1);
                    faceAux[i][1] = {}; //just to be sure...
                }
                Faces.emplace_back(faceAux);
            }
            else // it is assumed two '/' per face
            {
                for(unsigned i=0; i<3; ++i)
                {
                    std::getline(face, vertex, ' ');

                    std::stringstream vertexSS{vertex};
                    std::getline(vertexSS, c, '/');
                    std::getline(vertexSS, t, '/'); // ignore it
                    std::getline(vertexSS, n, '/');

                    faceAux[i][0] =  static_cast<unsigned>(std::stoi(c)-1);
                    faceAux[i][1] =  static_cast<unsigned>(std::stoi(n)-1);
                }
                Faces.emplace_back(faceAux);
            }
        }
    }
    std::cout<<"[ OK ] Read obj file\n";

}


void OBJ::GetOpenGLArrays(std::vector<glm::vec3>& glCoordinates, std::vector<glm::vec3>& glNormals)
{

    for (const auto& face: Faces)
    {

        glCoordinates.emplace_back(Coordinates[face[0][0]]);
        glCoordinates.emplace_back(Coordinates[face[1][0]]);
        glCoordinates.emplace_back(Coordinates[face[2][0]]);

        if(!Normals.empty())
        {
            glNormals.emplace_back(Normals[face[0][1]]);
            glNormals.emplace_back(Normals[face[1][1]]);
            glNormals.emplace_back(Normals[face[2][1]]);
        }


    }
    std::cout<<"[ OK ] Build OpengGL arrays\n";

}




#endif // OBJ_H
