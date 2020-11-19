#include "CheckFunction.h"
#include "EnvironmentSetup.h"
#include "DecodingInput.h"
#include "ConflictGraph.h"
#include "randomHandler.h"
#include "DataDefinition.h"
#include "grasp.h"
#include "hgcc.h"
#include "CodingDecodingData.h"
#include "FuncsFromMain.h"

using namespace caching;

using namespace std;


/*data_matrix generateData(unsigned int m_files, unsigned int b_chuncks, unsigned int request_number)
{
    data_matrix data;

    vector<int> input;

    unsigned int input_size = 0;

    int offset = 0;

    //Read environment file
    FILE *env_file;
    env_file = fopen("/CachingFile/environment/environment_file","rb");

    fread(&input_size,sizeof(int),1,env_file);

    offset = request_number * (input_size + 1) * sizeof(int);

    fseek(env_file, offset, SEEK_SET);

    fread(&input_size,sizeof(int),1,env_file);

    for(unsigned int i=0; i<input_size; i++)
    {
        input.push_back(0);
        fread(&input.at(i),sizeof(int),1,env_file);
    }

    fclose(env_file);

    //Generate Data
    if (input.size() > 0)
    {
        cout << endl << "generating data interface.." << endl << "-------------" << endl << endl;
        data = fromArrayToMatrix(m_files, b_chuncks, input);
    }

    return data;
}*/



void generateCache (unsigned int id_utente, unsigned int n_utenti, unsigned int m_files, unsigned int b_chuncks, data_matrix &data, FILE *cache_file, string pathFolder)
{

    unsigned int id_requested_file = data.Q[id_utente];

    cout << endl << "User id: " << id_utente;
    cout << endl << "Request File id: " << id_requested_file << endl;

    /*-------------------------MODIFICATION FOR CORTEXLAB---------------------*/
    string pathFiles = "../repository"; //"/CachingFile/repository"
    vector<string> files;
    files = getDirectoryFiles(pathFiles);

    if (files.empty())
    {
        cout << endl << "Repository folder is empty or it not exist." << endl << endl;
        exit(0);
    }

    /*Sort file by name*/
    sort(files.begin(), files.end());

    cout << endl << "Request File Name: " << files.at(id_requested_file) << endl << endl;




    cout << endl << " ----------------- Making cache ----------------- " <<endl;


    unsigned int n_package_remains = 0;
    makeCache(id_utente, data.Ind[id_utente], files, b_chuncks, m_files, &n_package_remains, id_requested_file, pathFolder);


    cout << endl << " ----------------- Cache Done ! ----------------- " <<endl;


    //Write cache in binary file
    cout << endl << " ----------------- Writing Cache info ----------------- " <<endl;



    //string pathFileInfo = "/CachingFile/cache/UserCache/cache_info_" + to_string(id_utente);
    //const char * filepath = pathFileInfo.c_str();

    //FILE *cache_file;
    //cache_file = fopen(filepath,"wb");                                           //file aperto in scrittura binaria

    fwrite(&n_package_remains,sizeof(int),1,cache_file);                         //scrivo il numero di pacchetti rimanenti

    //fclose(cache_file);


    cout << endl << " ----------------- Cache is ready ----------------- " <<endl<<endl;

}




int main(){

    unsigned int n_utenti, m_files, b_chuncks, id_utente, L_request;

	n_utenti = 3;
    m_files = 20;
    b_chuncks = 100;
    L_request = 1;

    FILE *cache_file;

	printf("\n\nNumero Utenti: %d Numero Files: %d Numero Chunks: %d\n", n_utenti, m_files, b_chuncks);

    for(unsigned int id_utente=0; id_utente<n_utenti; id_utente++)
    {

        string pathFolder = "../cache/UserCache/user_" + to_string(id_utente); // "/CachingFile/cache/UserCache/user_"
        if (mkdir(pathFolder.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1)
        {
            cout << endl << "Error creating folder: " << pathFolder << endl;
            exit(0);
        }


        //env_file = fopen("/CachingFile/environment/environment_file","rb");

        string pathFileInfo = "../cache/UserCache/cache_info_" + to_string(id_utente); //"/CachingFile/cache/UserCache/cache_info_"
        const char * filepath = pathFileInfo.c_str();

        cache_file = fopen(filepath,"wb");

        cout << endl << "User id : " << id_utente;

        for(unsigned int i=0; i<L_request; i++)
        {
            data_matrix data;

            data = generateData(m_files, b_chuncks, i);

            cout << endl << "Request file " << i << " = " << data.Q[id_utente];

            generateCache(id_utente, n_utenti, m_files, b_chuncks, data, cache_file, pathFolder);
        }

        fclose(cache_file);
    }

	return 0;
}
