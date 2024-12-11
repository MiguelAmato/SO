#include <stdio.h>
#include <unistd.h> /* for getopt() */
#include <stdlib.h> /* for EXIT_SUCCESS, EXIT_FAILURE */
#include <string.h>
#include "defs.h"

/**
 * @brief  Make a copy of existing string allocating memory accordingly
 *
 * @param original
 * @return new string that is a clone of original
 **/
static inline char* clone_string(char* original)
{
	char* copy;
	copy=malloc(strlen(original)+1);
	strcpy(copy,original);
	return copy;
}


/**
 * @brief Parse /etc/passwd file and returns its contents as an array of entries.
 * The number of entries in the array is returned vie the nr_entries parameter.
 *
 * @param options
 * @param nr_entries
 * @return array of entries (passwd_entry_t*) and entry count (nr_entries parameter)
 */
passwd_entry_t* parse_passwd(struct options* options, int* nr_entries)
{
	FILE* passwd;
	char line[MAX_PASSWD_LINE+1];
	passwd_entry_t* entries;
	passwd_entry_t* cur_entry;
	int line_count=0;
	char* token;
	char* lineptr;
	token_id_t token_id;
	int entry_idx;

	if (options->infile != NULL) {
		if ((passwd=fopen(options->infile,"r"))==NULL) {
			fprintf(stderr, "Infile could not be opened: ");
			perror(NULL);
			return NULL;
		}
	}
	else if ((passwd=fopen("/etc/passwd","r"))==NULL) {
		fprintf(stderr, "/etc/passwd could not be opened: ");
		perror(NULL);
		return NULL;
	}

	/* Figure out number of lines */
	while (fgets(line, MAX_PASSWD_LINE + 1, passwd) != NULL)
		line_count++;

	/* Rewind position indicator*/
	fseek(passwd,0,SEEK_SET);

	entries=malloc(sizeof(passwd_entry_t)*line_count);
	/* zero fill the array of structures */
	memset(entries,0,sizeof(passwd_entry_t)*line_count);

	/* Parse file */
	entry_idx=0;
	while (fgets(line, MAX_PASSWD_LINE + 1, passwd) != NULL) {
		/* Point to the beginning of the line */
		lineptr=line;
		token_id=LOGIN_NAME_IDX;
		cur_entry=&entries[entry_idx];

		while((token = strsep(&lineptr, ":"))!=NULL) {
			switch(token_id) {
			case LOGIN_NAME_IDX:
				strcpy(cur_entry->login_name,token);
				break;
			case ENCRYPTED_PASS_IDX:
				cur_entry->optional_encrypted_passwd=strdup(token);
				break;
			case UID_IDX:
				if (sscanf(token,"%d",&cur_entry->uid)!=1) {
					fprintf(stderr, "Couldn't parse UID field in line %d of /etc/passwd. token is %s\n",entry_idx+1, token);
					return NULL;
				}
				break;
			case GID_IDX:
				if (sscanf(token,"%d",&cur_entry->gid)!=1) {
					fprintf(stderr, "Couldn't parse GID field in line %d of /etc/passwd. token is %s\n",entry_idx+1, token);
					return NULL;
				}
				break;
			case USER_NAME_IDX:
				cur_entry->user_name=strdup(token);
				break;
			case USER_HOME_IDX:
				cur_entry->user_home=strdup(token);
				break;
			case USER_SHELL_IDX:
				/* remove new line from token */
				token[strlen(token)-1]='\0';
				cur_entry->user_shell=strdup(token);
				break;
			default:
				break;
			}
			token_id++;
		}
		if (token_id!=NR_FIELDS_PASSWD) {
			fprintf(stderr, "Could not process all tokens from line %d of /etc/passwd\n",entry_idx+1);
			return NULL;
		}

		entry_idx++;
	}
	(*nr_entries)=line_count;
	return entries;
}

/**
 * @brief Free up the array of entries, including the 
 * memory of dynamically allocated strings
 * 
 * @param entries 
 * @param nr_entries 
 */
static void free_entries(passwd_entry_t* entries, int nr_entries)
{
	int i=0;
	passwd_entry_t* entry;

	for (i=0; i<nr_entries; i++) {
		entry=&entries[i]; /* Point to current entry */
		free(entry->optional_encrypted_passwd);
		free(entry->user_name);
		free(entry->user_home);
		free(entry->user_shell);
	}

	free(entries);
}

/**
 * @brief This function takes care of invoking the parser, 
 * and then displays the contents of /etc/passwd based on the 
 * selected output mode
 * 
 * @param options 
 * @return int 
 */
static int show_passwd(struct options* options)
{
	int nr_entries;
	int i;
	passwd_entry_t* entries=parse_passwd(options,&nr_entries);

	if (!entries)
		return EXIT_FAILURE;

	for (i=0; i<nr_entries; i++) {
		passwd_entry_t* e=&entries[i]; /* Point to current entry */
		switch (options->output_mode) {
		case VERBOSE_MODE:
			fprintf(options->outfile,"[Entry #%d]\n",i);
			fprintf(options->outfile,"\tlogin=%s\n\tenc_pass=%s\n\t"
			        "uid=%d\n\tgid=%d\n\tuser_name=%s\n\t"
			        "home=%s\n\tshell=%s\n",
			        e->login_name, e->optional_encrypted_passwd,
			        e->uid, e->gid,e->user_name,
			        e->user_home, e->user_shell);
			break;
		case PIPE_MODE:
			fprintf(options->outfile,"|%s|%s|%d|%d|%s|%s|%s|\n",
			        e->login_name, e->optional_encrypted_passwd,
			        e->uid, e->gid,e->user_name,
			        e->user_home, e->user_shell);
		case C_MODE:
			fprintf(options->outfile,"%s, %s, %d, %d, %s, %s, %s\n",
			        e->login_name, e->optional_encrypted_passwd,
			        e->uid, e->gid,e->user_name,
			        e->user_home, e->user_shell);
		}

	}
	free_entries(entries,nr_entries);
	return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{
	int retCode, opt;
	struct options options;

	/* Initialize default values for options */
	options.outfile=stdout;
	options.infile = "/etc/passwd";
	options.output_mode=VERBOSE_MODE;

	/* Parse command-line options */
	while((opt = getopt(argc, argv, "hvpco:i:")) != -1) {
		switch(opt) {
		case 'h':
			fprintf(stderr,"Usage: %s [ -h | -v | -p | -o <output_file> | -i <inputfile> ]\n",argv[0]);
			exit(0);
		case 'v':
			options.output_mode=VERBOSE_MODE;
			break;
		case 'p':
			options.output_mode=PIPE_MODE;
			break;
		case 'c':
			options.output_mode=C_MODE;
			break;
		case 'o':
			if ((options.outfile=fopen(optarg,"wx"))==NULL) {
				fprintf(stderr, "The output file %s could not be opened: ",
				        optarg);
				perror(NULL);
				exit(EXIT_FAILURE);
			}
			break;
		case 'i':
			if (optarg != NULL) {
				options.infile = optarg;
			}
			else {
				fprintf(stderr, "The input file could not be opened: ");
				perror(NULL);
				exit(EXIT_FAILURE);
			}
			break;

		default:
			exit(EXIT_FAILURE);
		}
	}

	retCode=show_passwd(&options);
	exit(retCode);
}