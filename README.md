## Project Description

The code above will read data from a file, sort it, and output the sorted values. 

**Note:** This will only work on files with comma separated clusters of ASCII characters. The characters need to be either strictly numerical or strictly alphabet data. The file must be a .txt file.

### Output

The code will read all of the data, determine if it is numeric or alphabet data, sort it accordingly, and output the sorted data to STDOUT.

### To Run

**./fileSort -\<flag> ./\<File name>**

### Flags

- \-i: Tells the program to implement insertion sort
- \-q: Tells the program to implement quick sort

---
## Description of algorithms

- **Insertion sort:** for every token A in the dataset, if there is a token B that belongs in front of A, it will swap their values, essentially inserting token A where it needs to go. This process is repeated for every token in the dataset until it is sorted. This algorithm is rather inefficient, operating at (worst case) O(n^2) efficiency. A more detailed explanation of this algorithm can be found here: https://medium.com/@hitherejoe/algorithms-insertion-sort-eec0e245ec42

- **Quick sort:** using the first token in the list as a pivot, it will recursively divide the list of tokens into smaller and smaller chunks, sort the small chunks, then progressively builds the list back up. This algorithm, similar to insertion sort, operates at (worst case) O(n^2). A more detailed explanation of this algorithm can be found here: https://medium.com/karuna-sehgal/a-quick-explanation-of-quick-sort-7d8e2563629b
