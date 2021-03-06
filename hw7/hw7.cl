
int isOrdered(global const char * str,int a,int b) {
     while(str[a] != 0 && str[b] != 0) {
        if(str[a] == str[b]) {
            a++;
            b++;
        } else if(str[a] > str[b] ) {
            return -1;
        } else {
            return 1;
        }
    }

    if (str[a] == 0 && str[b] == 0) return 0;
    else if(str[b] == 0) return -1;
    else return 1;
}



kernel void sort(global const char* str, global const int *index,
                 const int size,global int *ret) {
    const int id = get_global_id(0);
    const int currentIndex = index[id];
    
    //char *currentStr = str[currentIndex];
    int pos = 0;
    for(int i =0;i < size;i++) {
        int comIndex = index[i];
        int comp = isOrdered(str,currentIndex,comIndex);
        bool upDown = ((comp == -1)|| 
		( (comp==0) && (comIndex > currentIndex)  ) );
        int count = 0;
        while(str[comIndex+count] != 0) {
            count ++;
        }
	count++;
        pos += (upDown ? count:0);
    }
    ret[id] = pos;
}
