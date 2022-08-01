// Documentation: https://librdf.org/raptor/api/

#include <raptor2.h>
#include <stdio.h>
#include <string.h>

void pprint_term(raptor_term *term) {
  // Note: There is a turtle string function
  // src: https://librdf.org/raptor/api/raptor2-section-triples.html#raptor-term-to-turtle-string
  // Print type as string
  switch(term->type) {
    case RAPTOR_TERM_TYPE_URI:
      printf("[URI] ");
      break;
    case RAPTOR_TERM_TYPE_LITERAL:
      printf("[LIT] ");
      break;
    case RAPTOR_TERM_TYPE_BLANK:
      printf("[BLK] ");
      break;
    default:
      printf("[UNK] ");
      break;
  }
  // Print value as string
  printf(raptor_term_to_string(term));
  printf("\n");
}

void handler_function(void *user_data, raptor_statement *triple) {
  raptor_term *subject = triple->subject;
  raptor_term *predicate = triple->predicate;
  raptor_term *object = triple->object;
  printf("SUBJECT   : ");
  pprint_term(subject);
  printf("PREDICATE : ");
  pprint_term(predicate);
  printf("OBJECT    : ");
  pprint_term(object);
  printf("----------------\n");
}

int main(int argc, char *argv[]) {
  // 0. Variable declaration
  // Raptor's world object. It is used for initialize and finish the process
  raptor_world *world = NULL;
  // Raptor's parser object
  raptor_parser *rdf_parser = NULL;
  // Uri string
  unsigned char *uri_string;
  // Raptor's uri object
  raptor_uri *uri, *base_uri;
  // 1. Initialize the raptor world object
  world = raptor_new_world();
  // 2. Initialize the raptor parser object for Turtle
  rdf_parser = raptor_new_parser(world, "turtle");
  // 3. Link the raptor parser object to a handler function
  // Note: Second argument is the user data, which is NULL in this case
  raptor_parser_set_statement_handler(rdf_parser, NULL, handler_function);
  // 4. Handle filename as URI
  uri_string = raptor_uri_filename_to_uri_string(argv[1]);
  uri = raptor_new_uri(world, uri_string);
  base_uri = raptor_uri_copy(uri);
  // 5. Execute file parsing
  // Note: Exists a file stream parser, but it is not used here
  raptor_parser_parse_file(rdf_parser, uri, base_uri);
  // 6. Free every used resource
  raptor_free_parser(rdf_parser);
  raptor_free_uri(base_uri);
  raptor_free_uri(uri);
  raptor_free_memory(uri_string);
  raptor_free_world(world);

  return 0;
}
