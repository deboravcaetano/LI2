int sufPref (char s1[], char s2[]) {
    int i1, i2 = 0;
    for(i1 = 0; s1[i1]; i1++) {
        if(s1[i1] == s2[i2]) i2++;
        else i2 = 0;
    }
    return i2;
}