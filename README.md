# *projectCACHE* Library & Implementation Guide


**gr-projectCACHE** is a GNU Radio module that combines, inter alia, novel caching strategies and polar codes for wireless communications. It uses its dedicated software library `project_caching`.  


---

## I. Notes related to the source code
---

#### About Custom Software Entities in `project_caching` library:
- Definitons location:
    - Typestruct definitions:
        - Defined in `DataDefinition.h` :
            - `cf_data, data_matrix, NODE, header_transmission, ...`
    - Class definitions:
        - `PC` defined in `PC.*`
    - Functions:
        - Defined in `FuncsFromMain.*` :
            - `colorRienumeration()`
        - Defined in `Conversions.*` :
            - **All** `conv_*()` functions.
        - Defined in `PolarDec_b_impl.*` itself:
            - `update()`, `snr()`, `decode_header()`, `reinitialize()`


- Terminology note:
    - PC_**w** : weak | PC_**s** : strong
    - **GRASP**: Greedy Randomized Adaptive Search Procedure. Basically an optimization algorithm to solve a optimization problem when there is no mainstream method. The improvement comes with a **local search** \\   
    <span style="color:red">No need to read more</span>

#### Transmitted Packet Structure:

- *FIGURE TO BE DONE (draw.io)*

---
## II) GNU Radio implementation
---
### A) Encoder _(polarEnc_b_impl)_

1. **Initialization** in private constructor (as usual), all variables declared in header
    1. Initialize fundamental types variables and arrays (int, bool, float,...)
    2. Initialize custom variables then construct Polar Codes (PC). The latter is done as follows:
        1. `PC_w.constructPC()` construct **Weak PC**. Consists of 3 functions:
            - `initPC(n,k,SNR)`. *starts with `setPC(n,k,SNR)`*
            - `constructGenMatrix()` Construct Generator Matrix. *uses `kroneckerProduct()`*
            - `arrangeBits()`. Based on Bhattacharyya parameter, _need to look up_.
        2. Construct **Strong PC** `PC_s` based on `PC_w` from step 1. Consists of 4 functions:
            - `initPC(n,k,SNR)`. *the same as the weak*
            - `setGenMatrix(PC_w.genMatrix)` Copies Generator Matrix of `PC_w`.
            - `setRn(PC_w.Rn)` Copies Permutation Vector of `PC_w`.
            - `setArrangedBits(PC_w.arragedBits)` Copies arranged bits of `PC_w`.
                - Note: `Z` array is just for computation, the actual arranged bits are in `arragedBits[i]` (TYPO included).
2. **Runtime** (Inside `work()`)
    1. Generation processes (and prints). They are executed once, `bool d_gen` makes sure of that. In the following order:
        - `generateData()`
        - `conflictGraphGenerator()`
        - if GRASP coloring is active:
           - `graspGraphColoring()`
           - `colorRienumeration()`
        - If coloring is successful (i.e. `int d_n_col` > 0) we can start coding:
           - `codingData()`
           - `MaxBipartiteGraph()`
           - `codingDataPolar()`
           - `TX_PC_Pack()` : Generate packets to be transmitted and stores them in `d_transmission[]`. Some notes:
              - `d_transmission[]` is a matrix and is NOT the output of the block. `d_transmission1[]` is the actual output vector and is made of vectors from `d_transmission[]`
              - The packet `d_transmission1[]` contains the Header, the Strong/Weak 2 information bytes and the payload.
        - Formatting and type conversions
        - HW adaptation : Repeat the last packet to make sure that all the buffer is processed
        - Add tags
    2. Check how many samples were produced in GR block output:
         - If all the packets were transmitted, `use cleanVar()` & **stop flowgraph**.
         - If not, continue and produce the minimum between what's left of the generated packet or the GR block output buffer, i.e. `min(d_transmission1-d_offset,noutput_items)`
         - call `work()` function again (loop all over) and skip step 1 since `d_gen = false`.


---
### B) Decoder _(PolarDec_b_impl)_

0. **Functions:**
    - `calculate_output_stream_length()` <span style="color:red">return 0 ALWAYS ???</span>
    - `update()`: Updates `d_y1` and `d_y2` which are used in `snr()`
        - `update(d_sSymb, in)`:
            - `d_sSymb` is actually input0 size
            - `in[]` is input0 (pointer)
        - `snr()` computation :

            <img src="https://i.imgur.com/QjldTTU.jpg" width="65%">
        - <span style="color:red">_LOGIC REMARK_</span> : returns first `int`  argument ? and isn't affected to anything ? We don't care!
        <!-- - <span style="color:red">_SYNTAX REMARK_</span> : First argument in definition is `noutput_items`, but in `work()` we use `d_sSymb` which points to `ninput_items[0]` -->
        - <span style="color:red">_COMPUTE REMARK_</span> : We could remove the `abs()` commands since we compute powers with even parity (2 and 4)
    - `decode_header()` :
        - uses `conv_4QPSKsymb_to_int()` and type conversions for each reading step
        - read header length
        - read ID demands
        - ... <span style="color:red">_READING UNFINISHED_</span>
    - `reinitialize()` : Restore header and main variables




1. **Initialization** is the same as the Encoder, with more stuff:
    - <span style="color:red">_QUESTION_</span> : If `d_id_user < d_n_users`: readCacheInfo() ? Something about the last user ? (line 72)
    - Then Create & set directories


2. **Runtime** inside `work()`:
    1. Initialize some local variables
    2. Get `packet_num` tags
        - <span style="color:red">_QUESTION_</span> : What are they used for ?
    3. Estimate SNR. Executed in every call of `work()`. The steps are :
        - Run `update()` then `snr()`
        - SNR Averaging.
            - Computation:
                - <span style="color:red"> *IMG TO BE UPLOADED* </span>
            - Used in cases {3,4,5} in the **State Machine (SM)** *(and for debug print)*
    4. Demodulate 8 QPSK symbols (ID of the Header) :
        - Run `conv_8QPSKsymb_to_int()` and compute Header ID `d_id_spack`
        - Check packet nature based on Header ID and update SM *(controlled by `d_case`)*:
            - If `d_id_spack == d_id_expected`, it means Rx packet was expected --> **Successful transmission**.
                - Respective SM cases `d_case = {1,2,3}`
            - If `d_id_spack != d_id_expected`, it means Rx packet **wasn't expected**.
                - Respective SM cases `d_case = {0,4,5}`

    5. <span style="color:red"> *INVESTIGATE STATE MACHINE* </span>
    6. Check Remaining packets
    7. Repeat

---







\
\
\
\
\
\
