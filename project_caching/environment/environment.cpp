#include "CheckFunction.h"
#include "EnvironmentSetup.h"
#include "DecodingInput.h"
#include "ConflictGraph.h"
#include "randomHandler.h"
#include "DataDefinition.h"
#include "grasp.h"
//#include "hgcc.h"
#include "CodingDecodingData.h"


using namespace caching;

using namespace std;

int main(){

     double alpha = 1.0;                                                         //ALPHA PARAMETER for Zipf distribution



    unsigned int n_utenti = 4;                                                 //number of user
    unsigned int n_user_s = 0;
    unsigned int m_files = 20;                                                  //number of file
    unsigned int b_chunks = 200;                                                //number of chunks

    unsigned int L_request = 1;                                                //number of demands for each user

    unsigned int M_cache = (10 * m_files / 100);                                //dimensione della cache 10% del catalogo
    unsigned int B_cache = M_cache * b_chunks;                                  //numero totale di pacchetti int cache




    int index_input;

    double *probs_vec;
    double sum=0;
    double sum_check;

    int *M_vec;
    int chunk_check;

    double **probs;                                                             //Matrice n_utenti X m_files inizializzata sotto
	int **M;                                                                    //Matrice n_utenti X m_files inizializzata sotto


    vector<int> input;                                                      //vettore prodotto da environment setup
    vector<int> memory_per_user(n_utenti, 0);

    FILE *env_file;

	printf("\n\nNumero Utenti: %d Numero Files: %d Numero Chunks: %d\n", n_utenti, m_files, b_chunks);


    probs_vec = (double *) malloc (m_files * sizeof(double));
    check_memory_double_allocation_1D(probs_vec, "Allocation 1D Probs.");

    //Probs_vec[i] = probabilità dell'i-esimo file di essere richiesto
    for (unsigned int i=0; i<m_files; i++)
    {
        double j = i+1;
        probs_vec[i] = (1/pow(j,alpha));
        sum += probs_vec[i];
    }
    for(unsigned int i=0; i<m_files; i++)
    {
        probs_vec[i] = probs_vec[i] / sum;
        sum_check += probs_vec[i];
        cout << endl << "Probs at (" << i << ") = " << probs_vec[i] << endl;
    }
    /*if(sum_check != 1)
    {
        cout << endl << "Sum of probability = " << sum_check << endl;
        cout << endl << "Error: sum of probability is not equal to 1!!!" << endl;
        exit(0);
    }*/



    probs = (double **) malloc (n_utenti * sizeof(double *));
    check_memory_double_allocation_2D(probs, "Allocation 2D Probs.");

    for (unsigned int i=0; i<n_utenti; i++){
        probs[i] = (double *) malloc (m_files * sizeof(double));
        check_memory_double_allocation_1D(probs[i], "Allocation 1D Probs.");
    }

    //probs[i][j] = probabilità dell'utente i-esimo di richiedere il file j-esimo
    for(unsigned int i=0; i<n_utenti; i++)
    {
        memcpy(probs[i],probs_vec,m_files * sizeof(double));
    }



    //FUNZIONI PER ARROTONDARE
    //round() round to nearest integer
    //ceil() round to next integer
    //floor() round to previous integer


    M_vec = (int *) malloc (m_files * sizeof(int));
    check_memory_allocation_1D(M_vec, "Allocation 1D M_vec.");
    for (unsigned int i=0; i<m_files; i++)
    {
        M_vec[i] = round(B_cache * probs_vec[i]);
        //cout << endl << "M_vec at (" << i << ") = " << M_vec[i] << endl;
        chunk_check += M_vec[i];
    }



    M = (int **) malloc (n_utenti * sizeof(int *));
    check_memory_allocation_2D(M, "Allocation 2D M.");

    for (unsigned int i=0; i<n_utenti; i++){
        M[i] = (int *) malloc (m_files * sizeof(int));
        check_memory_allocation_1D(M[i], "Allocation 1D M.");
    }


    //M[i][j] = numero di pacchetti del j-esimo file che l'i-esimo utente ha in memoria
    for(unsigned int i=0; i<n_utenti; i++)
    {
        memcpy(M[i],M_vec,m_files * sizeof(int));
    }
    //strong users have cache memory equal to 0
    for(unsigned int i=(n_utenti-n_user_s); i<n_utenti; i++)
        for(int k=0; k <m_files; k++)
            M[i][k] = 0;
    /*cout << "The memory is filled as follows: " << endl;
    for(int i=0; i<n_utenti; i++){
        for(int k=0; k <m_files; k++)
            cout << M[i][k] << " ";
        cout << endl;
    }*/

    //Create the "input" vector
    input = setEnvironment(n_utenti, m_files, b_chunks, probs, M, memory_per_user);
    for (int i = 0; i < n_utenti; ++i)
        cout <<  memory_per_user.at(i) << ", ";
    cout << endl;

    //write the "input" vector in a file
    unsigned int input_dim = input.size();

    //cout << "Input size : " << input_dim;

    env_file = fopen("./environment_file","wb");

    /*fwrite(&input_dim,sizeof(int),1,env_file);

    for(unsigned int i=0; i<input_dim; i++)
    {
        fwrite(&input.at(i),sizeof(int),1,env_file);
    }*/


    /******************************************************************************************/
    /******************************************************************************************/
    /******************************************************************************************/
    vector <int> Q_array(n_utenti, 0);
    srand(time(NULL));
    for(unsigned int k=0; k<L_request; k++)
    {

        //FUNZIONE randomQvector
        int idx_rand;

        /******************************************************************************************/
        unsigned long int randSeed = 0;
        gsl_rng *r;
        gsl_ran_discrete_t *rand_disc;

        //srand(time(NULL));                    /* initialization for rand() */
        randSeed = rand();                    /* returns a non-negative integer */

        gsl_rng_env_setup();
        gsl_rng_default_seed = randSeed;

        r = gsl_rng_alloc(gsl_rng_default);

        /******************************************************************************************/
        /*For each user generate a random request*/
        for (unsigned int i=0; i<n_utenti; i++)
        {
            rand_disc = gsl_ran_discrete_preproc (m_files, probs[i]);
            idx_rand =  gsl_ran_discrete (r, rand_disc);

            if (idx_rand >= m_files || idx_rand < 0)
            {
                printf("\nError: Random Index.\n");
                exit(0);
            }

            Q_array.at(i) = idx_rand;
            //cout << "Idx_rand " << idx_rand << endl;
        }


        /*for(unsigned int i=0; i<n_utenti; i++)
        {
            cout << "Request at " << i << " : " << Q_array.at(i) << endl;
        }
        cout << endl;*/


        index_input = input.size() - 1;

        //Funzione modifica vettore di input
        for(unsigned int i=0; i<n_utenti; i++)
        {
            index_input --;
            input.at(index_input) = Q_array.at(i);
            index_input --;
            index_input = (index_input - ( memory_per_user.at(i) * 2 ) ) - 1;
        }


        input_dim = input.size();
        //cout << "Input size : " << input_dim;
        cout << "The input size is: " << input_dim << endl;
        cout << "The input vector is: " << endl;

        //for strong users with no cache, print: -the nb request -the id file -cache size
        for(int i=0; i<n_user_s; i++)
            for(int j=0; j<3; j++)
                cout << input.at(3*i+j) << " ";
        cout << endl << endl;

        //for weak users, print -the nb request -the id requested file
        //-the memory size in chunks, and -the chunks to cache (file id, chunck id)
        for (int i = 0; i < (n_utenti-n_user_s); i++){
            for (int j = 0; j < (memory_per_user.at(i)*2+3); j++){
                cout << input.at(3*n_user_s+(memory_per_user.at(i)*2+3)*i+j) << " ";
            }
            cout << endl << endl;
        }

        /*for(int i=0; i<input_dim; i++)
            cout << input.at(i) << " ";*/

        fwrite(&input_dim,sizeof(int),1,env_file);

        for(unsigned int i=0; i<input_dim; i++)
        {
            fwrite(&input.at(i),sizeof(int),1,env_file);
        }

    }
    /******************************************************************************************/
    /******************************************************************************************/
    /******************************************************************************************/

    fclose(env_file);

    cout<<endl<<"Environment scritto correttamente!!"<<endl<<endl;


	return 0;
}
