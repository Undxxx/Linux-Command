#include "ls.h"
void print_mode(__mode_t mode){
    
    char res[10] = {"----------"};
    char type = '-';

     if(S_ISDIR(mode)){
        type = 'd';
    }else if(S_ISBLK(mode)){
        type = 'b';
    }else if(S_ISCHR(mode)){
        type = 'c';
    }else if(S_ISFIFO(mode)){
        type = 'p';
    }else if(S_ISSOCK(mode)) {
        type = 's';
    }else if(S_ISLNK(mode)){
        type = 'l';
    }else if(S_ISREG(mode)){
        type = '-';
    }
    res[0] = type;
    if(S_IRUSR & mode){
        res[1] = 'r';
    }
    if(S_IWUSR & mode){
        res[2] = 'w';
    }
    if(S_IXUSR & mode){
        res[3] = 'x';
    }
    if(S_IRGRP & mode) {
        res[4] = 'r';
    }
    if(S_IWGRP & mode) {
        res[5] = 'w';
    }
    if(S_IXGRP & mode) {
        res[6] = 'x';
    }
    if(S_IROTH & mode) {
        res[7] = 'r';
    } 
    if(S_IWOTH & mode) {
        res[8] = 'w';
    }
    if(S_IXOTH & mode) {
        res[9] = 'x';
    }
    printf("%s ",res);
}

void print_user_and_group(__uid_t uid, __gid_t gid){
    printf("%-8s ",getpwuid(uid)->pw_name);
    printf("%-8s ",getgrgid(gid)->gr_name);
}
void print_time(struct tm* local_time){
    char months[12][4] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
    printf("%s ",months[local_time->tm_mon]);
    if(local_time->tm_mday>9){
        printf("%d ",local_time->tm_mday);
    }else{
        printf(" %d ",local_time->tm_mday);
    }
    printf("%02d:%02d ",local_time->tm_hour,local_time->tm_min);
    
}
void ls_one(struct stat* statbuf,struct dirent *entry,int i_option,int l_option,int d_option,char* d_name){
    if(i_option==1){
        printf("%-17ld ", statbuf->st_ino);
    }
    if(l_option==1){
        print_mode(statbuf->st_mode);
        printf("%-2ld ", statbuf->st_nlink);
        print_user_and_group(statbuf->st_uid,statbuf->st_gid);
        printf("%-8ld ", statbuf->st_size);
        print_time(localtime(&statbuf->st_mtime));
    }
    if(d_option==1){
        printf("%s",d_name);
    }else{
        printf("%s",entry->d_name);
    }
    if(l_option==1){
        printf("\n");
    }else{
        printf(" ");
    }
}
void root_ls(char *target_dir,int options[]){
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    if ( (dp = opendir(target_dir)) == NULL ){
        printf("Error opening directory!");
        return;
    }
    //处理-d
    if(options[4]==1){
        entry = readdir(dp);
        char tmp[1000];
        strcpy(tmp, target_dir);
        strcat(tmp, "/");
        strcat(tmp, entry->d_name);
        if(stat(tmp, &statbuf)==-1){
            printf("Error\n");
        }
        ls_one(&statbuf,entry,options[2],options[0],options[4],target_dir);
        return;
    }
    //-R
    if(options[3]==1){
        printf("%s:\n", target_dir);
    }
    QNode *head = initQueue();
    QNode *tail = head;
    while ( (entry = readdir(dp)) != NULL ) {
        char tmp[1000];
        strcpy(tmp, target_dir);
        strcat(tmp, "/");
        strcat(tmp, entry->d_name);
        if(stat(tmp, &statbuf)==-1){
            printf("Error\n");
        }
        //-a
        if((entry->d_name[0]=='.'&&options[1]==0)){
           continue;
        }
        ls_one(&statbuf,entry,options[2],options[0],options[4],target_dir);
        if(options[3]==1){
            if(S_ISDIR(statbuf.st_mode)==1&&strcmp(entry->d_name,".")!=0&&strcmp(entry->d_name,"..")!=0){
                strcpy(tail->name,tmp);
                tail->next = initQueue();
                tail=tail->next;
            }
        }
    }
    if(options[0]!=1){
        printf("\n");
    }
    if(options[3]==1){
        printf("\n");
        while(head!=tail){
            root_ls(head->name,options);
            head = head->next;
        }
    }

    closedir(dp);
}

int main(int argc, char *argv[]){
    char *target_dir = malloc(1000);
    target_dir[0] = '.';
    int options[5] = {0,0,0,0,0}; //-l,-a,-i,-R,-d
    for(int i = 1;i<argc;i++){
        if(argv[i][0]!='-'){
            target_dir = argv[i];
        }else{
            for(int j = 1;j<6;j++){
                switch (argv[i][j])
                {
                case 'l':
                    options[0] = 1;
                    break;
                case 'a':
                    options[1] = 1;
                    break;
                case 'i':
                    options[2] = 1;
                    break;
                case 'R':
                    options[3] = 1;
                    break;
                case 'd':
                    options[4] = 1;
                    break;
                default:
                    break;
                }
            }
        }
    }
    root_ls(target_dir,options);
    
    return 0;
}