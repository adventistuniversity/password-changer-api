<?php
// $Id$

/**
* @file
* Store important private functions for the password changer API.
*
* This functions are back bone to the password changer API, it
* would work in conjunction with  password_changer_api.module.
* 
*/

/**
 * Create Exception coming from the password changer API.
 */
class passwordChangerApiExeption extends Exception {}

/*
 * Public function creates an status object and it sets it to default values.
 *
 * @param array $info
 *   string $info['status']
 *     Status fail, success or NULL
 *   string $info['message']
 *     Password changer api message.
 *   string $info['system_message']
 *    System message.
 *  
 * @return object $status
 *   string $status->status
 *     Store 'status', 'warning', 'error' or NULL if has not been set.
 *   string $status->message
 *     Stores the password changer api message, if it has not been set
 *     it stores an empty string.
 *   mixed $status->system_message
 *     Stores any message sent by the system, if it has not been set it
 *     stores an empty string. 
 */       

function password_changer_api_status($info = array()) {
 $status = new stdClass();
 $status->status = (isset($info['status']))? $info['status'] : NULL ;
 $status->message = (isset($info['message']))? $info['message'] : '' ;
 $status->system_message = (isset($info['system_message']))? $info['system_message'] : '' ;
 return $status;
}


function password_changer_api_get_register_modules()  {
 
 return variable_get('password_changer_api_register_modules', array());
 
}


/*
 * Public function to validate and populate the system array()
 *
 * Make sure that the title field is populated and create the
 * system array that is going to be store in the system.
 *
 * @param array $info
 *   string $system['#module_name']
 *     Name of the sub-module.
 *
 *   string $system['#title']
 *     Title to display in the form.
 *
 *   string $system['#description']
 *     Description of 
 *
 *   string $system['#optional_link']
 *    System message.
 *
 *   boolean $system['#must_change']
 *    If password must change else don't change the rest of the passwords.
 *
 *   array $system ['#complexity'] 
 *
 *       array '#max_length'
 *
 *             string '#validate_function' store the name of the function to validate
 *                     the password requirement.
 *                
 *             int    '#value' store a zero if does not have maximum length and positive number
 *                     for require length.
 *
 *             string '#patter' regex patter to check in password.
 *
 *             string '#error_message' error message to display on user.
 *             
 *       array '#min_length'
 *       
 *             string '#validate_function' store the name of the function to validate
 *                     the password requirement.
 *                
 *             int    '#value' store a zero if does not have minimum length and positive number
 *                     for require length.
 *
 *             string '#patter' regex patter to check in password.
 *
 *             string '#error_message' error message to display on user.
 *             
 *       array '#symbols'
 *       
 *             string '#validate_function' store the name of the function to validate
 *                     the password requirement.
 *                
 *             int    '#value' store a negative value if is disallow,
 *                     zero if does not care about content length and positive number
 *                     for a set length of values present.
 *
 *             string '#patter' regex patter to check in password.
 *
 *             string '#error_message' error message to display on user.
 *       
 *       array '#digit'
 *
 *            string '#validate_function' store the name of the function to validate
 *                     the password requirement.
 *                
 *             int    '#value' store a negative value if is disallow,
 *                     zero if does not care about content length and positive number
 *                     for a set length of values present.
 *
 *             string '#patter' regex patter to check in password.
 *
 *             string '#error_message' error message to display on user.
 *
 *       array '#upper_case'
 *
 *             string '#validate_function' store the name of the function to validate
 *                     the password requirement.
 *                
 *             int    '#value' store a negative value if is disallow,
 *                     zero if does not care about content length and positive number
 *                     for a set length of values present.
 *
 *             string '#patter' regex patter to check in password.
 *
 *             string '#error_message' error message to display on user.
 *       
 *       array '#lower_case'
 *
 *             string '#validate_function' store the name of the function to validate
 *                     the password requirement.
 *                
 *             int    '#value' store a negative value if is disallow,
 *                     zero if does not care about content length and positive number
 *                     for a set length of values present.
 *
 *             string '#patter' regex patter to check in password.
 *
 *             string '#error_message' error message to display on user.
 *  
 * @return array $system with default values
 *  
 *   
 */  

function password_changer_api_system($system = array(), $complexity = array()) {
 
 //If array is empty then return an empty array
 if (empty($system)) return array();
 
 //If array field name is not set throw an error.
 if (!isset($system['#title'])) {
    
    watchdog('[password_changer_api_inc][system function] ', 'Require field name is not set or is empty', WATCHDOG_ERROR);
    
    throw new passwordChangerApiExeption('Require field name is not set or is empty.');
 }
 
 if ( $system['#title'] == '' || is_null( $system['#title']) ) {
  
    watchdog('[password_changer_api_inc][system function] ', 'Require field name is empty or set to NULL.', WATCHDOG_ERROR);
    
    throw new passwordChangerApiExeption('Require field name is empty or set to NULL.');
 }
 
 //If array field is set to nothing throw an error
 
 //if array has content then return an array with the information.
 return array('#title'  => ucwords($system['#title']),
              '#description'  => (isset($system['#description'])) ? $system['#description'] : '' ,
              '#optional_link' => (isset($system['#optional_link'])) ? $system['#optional_link'] : '',
              '#must_change' => (isset($system['#must_change'])) ? $system['#must_change'] : FALSE,
              '#complexity' => (!empty($complexity)) ? $complexity : array(),
              );
}


/**
 * Private function to add a system would be able to uses the password
 * changer API. This is add to a persistent variable in the system
 * (password_changer_api_register_modules).
 *
 * @param array $system
 *  array $system [#module_name] (
 *  
 *    string $system['#title']
 *    The name of the system to be register, no space or symbols.
 *
 *   string $system['#title']
 *    The title that is going to display in the form.  
 *   
 *   string $system['#description']
 *    A short description of the new system that is going to be added.
 *
 *   string $system['#optional_link']
 *    An optional link where user can go and change the password in case
 *    the password changer fails to change password.
 *    
 *   string $system['#must_change']
 *    Flag that must change else don't change other system passwords.
 *
 *   array $system ['#complexity'] 
 *
 *       array '#max_length'
 *
 *             string '#validate_function' store the name of the function to validate
 *                     the password requirement.
 *                
 *             int    '#value' store a zero if does not have maximum length and positive number
 *                     for require length.
 *
 *             string '#patter' regex patter to check in password.
 *
 *             string '#error_message' error message to display on user.
 *             
 *       array '#min_length'
 *       
 *             string '#validate_function' store the name of the function to validate
 *                     the password requirement.
 *                
 *             int    '#value' store a zero if does not have minimum length and positive number
 *                     for require length.
 *
 *             string '#patter' regex patter to check in password.
 *
 *             string '#error_message' error message to display on user.
 *       
 *             
 *       array '#symbols'
 *       
 *             string '#validate_function' store the name of the function to validate
 *                     the password requirement.
 *                
 *             int    '#value' store a negative value if is disallow,
 *                     zero if does not care about content length and positive number
 *                     for a set length of values present.
 *
 *             string '#patter' regex patter to check in password.
 *
 *             string '#error_message' error message to display on user.
 *       
 *       array '#digit'
 *
 *            string '#validate_function' store the name of the function to validate
 *                     the password requirement.
 *                
 *             int    '#value' store a negative value if is disallow,
 *                     zero if does not care about content length and positive number
 *                     for a set length of values present.
 *
 *             string '#patter' regex patter to check in password.
 *
 *             string '#error_message' error message to display on user.
 *
 *       array '#upper_case'
 *
 *             string '#validate_function' store the name of the function to validate
 *                     the password requirement.
 *                
 *             int    '#value' store a negative value if is disallow,
 *                     zero if does not care about content length and positive number
 *                     for a set length of values present.
 *
 *             string '#patter' regex patter to check in password.
 *
 *             string '#error_message' error message to display on user.
 *       
 *       array '#lower_case'
 *
 *             string '#validate_function' store the name of the function to validate
 *                     the password requirement.
 *                
 *             int    '#value' store a negative value if is disallow,
 *                     zero if does not care about content length and positive number
 *                     for a set length of values present.
 *
 *             string '#patter' regex patter to check in password.
 *
 *             string '#error_message' error message to display on user.
 *      )       
 *
 */
function _password_changer_api_add_system($system = array(), $complexity = array()) {
 
 //Get register module, if not set return an empty array
 $register_modules = variable_get('password_changer_api_register_modules', array());
 
 //If the info variable is empty throw an error
 if (empty($system)) {
  //Log the issue in watchdog table in Drupal
  watchdog('[password_changer_api_inc][add_system function] ', 'The array() variable $info is empty.', WATCHDOG_ERROR);
  //Throw error
  throw new passwordChangerApiExeption('The array() variable $info is empty.');
 }
 
 
 //Make sure than the name of the module is set.
 if (!isset($system['#module_name'])) {
  //Log the issue in watchdog table
  watchdog('[password_changer_api_inc][add_system function] ', 'The module name is not set.', WATCHDOG_ERROR);
  //Throw error
  throw new passwordChangerApiExeption('The module name is not set.');
  
 }
 
 //Register new module, index is the module name and the description and optional
 //URL to change password.
 if (!isset($register_modules[$system['#module_name']])) {
  
  //Enter required information 
  $register_modules[$system['#module_name']] = password_changer_api_system($system, $complexity);
  
   //Allow other modules to add system information
   array_push($register_modules[$system['#module_name']] , module_invoke_all('password_changer_api_register_system_info'));
   
   //Allow other to alter system information CAUTION
   drupal_alter('password_changer_api_register_system_info', $register_modules[$system['#module_name']]);
   
   //Save it to persistent variable
   variable_set('password_changer_api_register_modules', $register_modules);
   
    
 }
 else {  
   //Log exception in watchdog
   watchdog('[password_changer_api_inc][add_system function] ', 'This module name is already register.', WATCHDOG_ERROR);
  
   //Throw error
   throw new passwordChangerApiExeption('This module name is already register.');
 }
}

/**
 * Private function to delete a system and it would not be able to use the
 * password changer API. This is deleted from a persistent variable in the system
 * (password_changer_api_register_modules).
 *
 * @param string $name
 *   The name of the system to be register, no space or symbols.
 *   
 */


function _password_changer_api_remove_system($module_name) {
 
 //Get persistent variable
 $register_modules = (array) variable_get('password_changer_api_register_modules', array());
  
 //Unregistered the angel module
 if (!empty($register_modules)) {
  //Log Uninstall to watchdog
  watchdog('[password_changer_api_inc][remove_system function] ',
           'Uninstall module %module_name', array('%module_name' => $module_name), WATCHDOG_ERROR);
  //Remove from array
  unset($register_modules[$module_name]);
  //Save to Drupal persistent variable
  variable_set('password_changer_api_register_modules', $register_modules);
 } 
}

/**
 * Private function to check if a given user exist in a system.
 *
 * @param array $system
 *   The system object with the system information.
 *
 * @param object $user_name
 *   The user_name that is going to change it's password.
 *
 *  @return boolean
 *    Return TRUE if user exist return FALSE if user does not exist.
 *   
 */

function _password_changer_api_check_user($system, $user_name) {

 //Get sub-module file directory and name from the system table
 $filename = db_query('SELECT `filename` FROM {system} WHERE `name` = :name AND `status` = \'1\' LIMIT 0,1',
                    array(':name' => $system))->fetchField();
  
 //Add the Drupal root directory 
 $include_file = DRUPAL_ROOT . '/'. $filename;
     
 //Check if file exist
 if(file_exists($include_file) && ($filename != '')) {
  //Include sub-module file to be able to call the hook if define 
  include_once($include_file);
  //Call the module hook to check the user define by the sub-module 
  return module_invoke($system, 'password_changer_api_check_user', $user_name);
 }

 return FALSE;
}

/**
 * Private function to change password in a given system.
 *
 * @param array $system
 *   The system object with the system information.
 *
 * @param object $user_name
 *   The user name of the user that is going to change it's password.
 *
 *  @return object $status
 *    Returns an object with the status of the password change.
 *     string $status->status
 *       Store 'success', 'failed' or NULL if has not been set.
 *     string $status->message
 *       Stores the password changer api message, if it has not been set
 *       it stores an empty string.
 *     mixed $status->system_message
 *       Stores any message sent by the system, if it has not been set it
 *       stores an empty string.
 *   
 */

function _password_changer_api_change_password($system, $user_name, $pass, $curr_pass = NULL) {
  
  watchdog('[password_changer_api_inc][change_password hook] ', 'System: !system and User name: !user_name',
            array ('!system' => $system, '!user_name' => $user_name), WATCHDOG_DEBUG );
  
 //credential that are going to be change to the submodules to change password.
 $credentials = array( 'user_name' => $user_name, 'password' => $pass, 'current_password' => $curr_pass);
 
 //Get sub-module file directory and file name by looking at the system table
 $filename = db_query('SELECT `filename` FROM {system} WHERE `name` = :name LIMIT 0,1',
                    array(':name' => $system))->fetchField();
 
 //Add the Drupal root directory 
 $include_file = DRUPAL_ROOT . '/' .$filename;
 
 
 //Check if file exist
 if(file_exists($include_file )) {
  //Include the sub-module file 
  include_once($include_file);
  
 }
  
  //Call the module hook to change passwords define by sub-module
  $status = module_invoke($system, 'password_changer_api_change_password', $credentials);
  
  // Create Drupal message has not been set don't display any message
  if (is_object($status) ) {
   
   
   //If there is an error   
   if( $status->status == 'error' ) {
   
    //Set message call for drupal to set the password
    drupal_set_message($status->message, $status->status);
    
    //Log status to the watchdog
    watchdog('[password_changer_api_inc][change_password hook] ', $status->message, WATCHDOG_ERROR);
    
   }
   else {
     //Set message
     watchdog('[password_changer_api_inc][change_password hook] ', $status->message, WATCHDOG_INFO);
      
     drupal_set_message($status->message, $status->status);
     
   }  
  }
}

/**
 * Save administrator message in Drupal.
 *
 * Save administrator message in Drupal persistent variable password_changer_api_admin_message.
 *
 * @param string $admin_message message to save as adminitrator message.
 *
 */

function password_changer_api_save_admin_message($admin_message  = '') {
 
 //Clean message up from XSS hacks
 $admin_message = filter_xss($admin_message);
 
 //Save  message on the password_changer_api_admin_message
 variable_set('password_changer_api_admin_message ', $admin_message);
 
}


/**
 * Helper function to make a simple curl.
 *
 * Make a simple curl call given the URL and and array of options.
 * Return the transfer if the CURLOPT_RETURNTRANSFER is set to in
 * the $options paramenters else return TRUE or FALSE.
 *
 * @param string $url the URL for the cURL request.
 * @param array $options an array with the options to cURL call
 *
 * @return return transfer if the CURLOPT_RETURNTRANSFER is set
 * to TRUE else return TRUE on success and FALSE on faliur.
 *
 * 
 *
 */

function password_changer_api_simple_curl_call($url = NULL, $options = array(), &$information = NULL)  {
 
 
  //Check if valid URL has been set  
  if (is_null($url)) {
   //Log Error
    watchdog('[password_changer_api_inc][simple_curl_call function] ', '(Function: password_changer_api_simple_curl_call)
            URL is set to NULL for your cURL call need to pass a valid  URL', WATCHDOG_ERROR);
    //Throw exeption
    throw new passwordChangerApiExeption('(Function: password_changer_api_simple_curl_call)
                                        URL is set to NULL for your cURL call need to pass a valid  URL');
  }
 
  //Check if URL is formated valid
  if( ! preg_match('|^http(s)?://[a-z0-9-]+(.[a-z0-9-]+)*(:[0-9]+)?(/.*)?$|i', $url) ) {
    //Log Error
    watchdog('[password_changer_api_inc][simple_curl_call function]', '(Function: password_changer_api_simple_curl_call)
            URL is not formated as valid URL', WATCHDOG_ERROR);
    //Throw exeption
    throw new passwordChangerApiExeption('(Function: password_changer_api_simple_curl_call)
                                        URL is not formated as valid URL');
  }
  
  //Make sure URL does not bring malicious code embeded
  $url = check_url($url);
    
  //Create a new cURL resource
  $ch = curl_init();
  //Set the url cURL resource 
  curl_setopt($ch, CURLOPT_URL, $url );
 
 //Go throug all the options a set them up
 foreach($options AS $option => $value)  {
   //Set cURL options
   curl_setopt( $ch, $option, $value);
 }
 
 //Execute curl handler
 $result = curl_exec($ch);

 //get information
 $information = curl_getinfo($ch);

 //Close cURL resource
 curl_close($ch);

 //return results from cURL call
 return $result;
 
}


/**
 * Generate a ramdom string.
 *
 * Generate a ramdom string with default length of eight.
 *
 * @param integer length The length of the string that is going to be returned.
 *
 * @return string Ramdom string.
 */

function password_changer_api_generate_random_string( $length = 8) {

    $chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    srand((double)microtime()*1000000);
    $i = 0;

    $string = '' ;

    while ($i <= $length) {
        $num = rand() % 33;
        $tmp = substr($chars, $num, 1);
        $string = $string . $tmp;
        $i++;
    }
    return $string;
}







