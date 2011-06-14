<?php
// $Id$
/**
* @file
* Store the core function for the password complexity checker for the password
* changer API.
*
* This functions are back bone to the password complexity checker for the password
* changer API, it would be work in conjunction with  password_changer_api.module.
* 
*/


function password_changer_api_check_maximum_length_requirement($password) {
 
 //Get register modules
 $register_modules = variable_get('password_changer_api_register_modules', array());
 
 //Get maximum length
 $min_maximum =  _password_changer_api_get_mim_maxlength( $register_modules );
 

 if (strlen($password) > $min_maximum['value'] && $min_maximum != 0) {
    
  watchdog('[password_changer_api_complexity][check_maximum_length_requirement]',
           'Password is longer than the maximum length' . $min_maximum['value'] . ' require by system ' . $min_maximum['system'], WATCHDOG_INFO);
    
  throw new passwordChangerApiExeption(t('Password is longer than the maximum length  @maxlen require by system @system',
                                     array( '@maxlen' => $min_maximum['value'],  '@system' => $min_maximum['system'] )));
  
  return array('status' => t('error'), 'message' => t('Password is longer than the maximum lent @maxlen require by system @system',
                                     array( '@maxlen' => $min_maximum['value'],  '@system' => $min_maximum['system'] )));
 }
 else {
     
  return array('status' => t('success'), 'message' => t('Password passed length requirements')); 
  
 }
}

/*
 * Function to check .
 *
 * @param string $password new password.
 * 
 */


function password_changer_api_check_minimum_length_requirement($password) {
  
 //Get register modules
 $register_modules = variable_get('password_changer_api_register_modules', array());

 //Get minimum length 
 $max_minimum  =  _password_changer_api_get_max_minlength( $register_modules );
 
 if (strlen($password) < $max_minimum['value'] && $max_minimum != 0) {
 
  watchdog('[password_changer_api_complexity][_check_minimum_length_requirement]', 'Password is shorter than the minimum length ' .
           $max_minimum['value'] . ' require by system ' . $max_minimum['system'], WATCHDOG_INFO);
  
  throw new passwordChangerApiExeption(t('Password is shorter than the minimum length  @minlen require by system @system',
                                         array( '@minlen' => $max_minimum['value'],  '@system' => $max_minimum['system'] )));
  
  return array('status' => t('error'), 'message' => t('Password is shorter than the minimum length @minlen require by system @system',
                                         array( '@minlen' => $max_minimum['value'],  '@system' => $max_minimum['system'])));
 }
 else  {
  
  return array('status' => t('success'), 'message' => t('Password passed length requirements')); 
 }
}

/*
 * Function to check that new password does not break complexity rules in any
 * of the register modules. Throw an passwordChangerApiExeption exception if it
 * breaks a complexity rule from any of the sub modules install.
 *
 * @param string $password new password.
 * 
 */

function _password_changer_api_check_property_requirement($password) {
 
 //Get register modules
 $register_modules = variable_get('password_changer_api_register_modules', array());
 
 
 //Default regex values 
 $default_regex_values = array('symbols' => '/[^A-Za-z0-9]/',
                               'digit' => '/[0-9]/',
                               'upper_case' => '/[A-Z]/',
                               'lower_case' => '/[a-b]/',
                             );
 
 //Default functions
 $default_validate_function = array('max_length' => 'check_maximum_length_requirement',
                                    'min_length' => 'check_minimum_length_requirement');
 
 
  if (isset($_SESSION['permission_modules'])) {
   
    //For modules that user has account 
    foreach ($_SESSION['permission_modules'] AS $index => $permission_module) {
     
     //Check if the module has complexity rules 
      if(isset($register_modules[$permission_module]['#complexity'])) {
      
        //Getting complexity
        $complexity = $register_modules[$permission_module]['#complexity'];
                
        //Foreach property in the complexity
        foreach ($complexity AS $property_name => $property_value ) {
   
           //Value set in the complexity array this could be a negative (deny)number
           //positive(require at least) number or zero (allow)
           $value = $property_value['value'];
           //Regex patter 
           $pattern = NULL;
           
           //If there is a regex function define for this property
           //set it to the pattern
           if (isset($default_regex_values[$property_name])) {
            $pattern = $default_regex_values[$property_name];
           }//If user has define a patter in the current property
           elseif (isset($property_value['pattern'])) {
            $pattern = $property_value['pattern'];
           }
           
           //If property has a pattern, value (negative number, positive number or zero)
           //and new password set then check the patter and throw error if necessary.
           if (!is_null($pattern) && isset($value) && isset($password)) {
           _password_changer_api_check_pattern( $password, $pattern, $value, $property_value['error_message'] );
           }
                 
           //Call functions define as default for max_length and min_length
           if (isset($default_validate_function[$property_name])) {
            //Throw an exception
            module_invoke('password_changer_api', $default_validate_function[$property_name], $password);
           }
           
           //Call validation function define by user
           if (isset($property_value['validate_function'])) {
            if (function_exists($property_value['validate_function'])) {
             //Call user function
             call_user_func_array($property_value['validate_function']);
            }  
          }
        
        // End of foreach
       }
      } 
    }
    
  }
  
}

/*
 * Helper function to check if value is require and is not in the password
 * or if value is deny and is part of the password.
 *
 * Helper function to detect if value is require at least a number of times
 * and not in the password or deny and is part of the password. Throw a
 * passwordChangerApiExeption if value deny and is password or require at
 * least a number of times and is not in the password.
 *
 * @param string $password new password.
 *
 * @param string $pattern a php regex pattern to check in the password.
 *
 * @param int $value -1 deny, 0 allow and positive integer for require at least
 * a number of times.
 * 
 * @param $error_message error message that function is going to throw.  
 * 
 */
function _password_changer_api_check_pattern($password, $pattern, $value, $error_message = 'Password complexity error.') {

 //Check if password required upper case
 $has_item = preg_match_all($pattern, $password, $matches);
  
 //if is require and it does not have this value at lea
 if ($value > 0 && $has_item < $value ) {
   watchdog('[password_changer_api_complexity][password_changer_api_check_pattern]' ,
            'Value is required and is not in the password.' .
            $error_message, WATCHDOG_ERROR);
   throw new passwordChangerApiExeption($error_message); 
 }//if is this value is deny and it has this value throw an error
 elseif ($value < 0 && $has_item > 0 ) {
   watchdog('[password_changer_api_complexity][password_changer_api_check_pattern]',
            'Value is deny and is in the password' .
            $error_message, WATCHDOG_ERROR);
   throw new passwordChangerApiExeption($error_message); 
 }
 
 
}

/*
 * Helper function to find the maximum number of the minimum password requirement.
 *
 * Helper function to find the maximum number withiEn all the minimum length values
 * of the register modules.
 *
 * @param array $register_module an with all the register modules and their properties.
 *
 * @return integer $max_minlength the greater value of the minimum length requirement.   
 *
 */
function _password_changer_api_get_max_minlength( $register_module ) {
 
 //Initialize the variable to empty values.
 $max_minlength = array();
 $max_minimum = 0;
 
 //Get the max minimum
 foreach ($register_module AS $module_name => $properties) {
   //Check if complexity is not empty
    if (!empty($properties['#complexity']['min_length']['value'])) {
      if ( $max_minimum < $properties['#complexity']['min_length']['value'] ) {
       $max_minimum = $properties['#complexity']['min_length']['value'];
       $max_minlength['value'] = $max_minimum;
       $max_minlength['system'] = $module_name;
      }  
    }
 }

 if (empty($max_minlength)) {  
   watchdog( '[password_changer_api_complexity][_get_max_minlength]', 'None maximum minimum value set in the sub-modules.', WATCHDOG_INFO);
 }
 else { 
   watchdog( '[password_changer_api_complexity][_get_max_minlength]', 'Maximum minimum value between sub-modules is ' . $max_minlength['value'] . ' from sub module ' . $max_minlength['system'] , WATCHDOG_INFO);
 }   
 
 //Return array with the max min and the module name
 return $max_minlength;
}

/*
 * Helper function to find the minimum number of the maximum length.
 *
 * Helper function to find the minimum number within all the maximum length values
 * of all the register modules.
 *
 * @param array $register_module an with all the register modules and their properties.
 *
 * @return array $min_maximum  the smallest value of the maximum length requirement.
 * 
 */
function _password_changer_api_get_mim_maxlength( $register_module ) {
 
 //Initialize the variable to empty values.
 $min_maxlength = array();
 $min_maxlen = 99;
 
  //Get the min max length
  foreach ($register_module AS $module_name => $properties) { 
   //Check if complexity is not empty
    if (!empty($properties['#complexity']['max_length']['value'])) {
      if ($min_maxlen > $properties['#complexity']['max_length']['value']) {
       $min_maxlen = $properties['#complexity']['max_length']['value'];
       $min_maxlength['value'] = $min_maxlen;
       $min_maxlength['system'] = $module_name;
      }    
    }
  }
 //Return array with the min maximum length and the module name
 //Log to watchdog.
 if ( empty($min_maxlength) ) {  
   watchdog( '[password_changer_api_complexity][_get_mim_maxlength]', 'None minimum maximum value set in the sub-modules.', WATCHDOG_INFO);
 }
 else { 
   watchdog( '[password_changer_api_complexity][_get_mim_maxlength]', 'Maximum Minimum value between sub-modules is ' . $min_maxlength['value'] . ' from sub module ' . $min_maxlength['system'] , WATCHDOG_INFO); 
 }             
 //return 
 return $min_maxlength;
}

/*
 * Check if there if there is global conflicts with password complexity rules.
 *
 * Check if there are any global conflicts with password complexity rules, if there
 * an conflict throw error. If there is only one system with password complexity rule
 * does  not check for conflicts.
 *
 * @return boolean FALSE if there is no conflicts to resolved or TRUE if there is
 * if there were no conflicts and throw and error if there were conflicts.
 * 
 */


function _password_changer_api_check_global_conflicts() {

 watchdog('[password_changer_api_complexity][_check_global_conflicts]', 'Checking global conflicts for password changer.');
 
 $register_modules = variable_get('password_changer_api_register_modules', array());
 
 //Check if more than one module has complexity rules
 if ($number_password_complexity = _password_changer_api_systems_with_complexity($register_modules) < 2) {
   return FALSE;
 }
 
 //Get maximum length
 $min_maximum =  _password_changer_api_get_mim_maxlength($register_modules);
 $max_minimum  =  _password_changer_api_get_max_minlength($register_modules);
   
 //Check if there is a conflict between the maximum length and the minimum length 
 //if maximum minimum value is greater than minimum maximum value throw an error
 if (isset($min_maximum['value'])&& isset($max_minimum['value'])) {
  
   if ($min_maximum['value'] < $max_minimum['value']) {
     
     watchdog('[password_changer_api_complexity][_check_global_conflicts]', '[CONFLICT] The maximum length of password 
     			 allow is smaller than the minimum password length allow.', WATCHDOG_ERROR);
     
     throw new passwordChangerApiExeption('[CONFLICT] The maximum length of password allow is smaller than the minimum password length allow.');
   }     
 }
 
 //Get potential conflicts. 
 $potential_conflicts = _password_changer_api_get_potential_conflicts($register_modules);
 
 //Check property is require and deny or allow and deny in different modules.
 foreach ($potential_conflicts AS $property => $requirements ) {
  
  if (isset($requirements['allow']) && isset($requirements['deny'])) {
   
    watchdog('[password_changer_api_complexity][_check_global_conflicts]', "Module(s) @allow allow @property and module(s) @deny deny @property this is causing a conflict.",
            array( '@allow' => trim($requirements['allow'], ','), '@property' => $property, '@deny' => trim($requirements['deny'], ',')),
                  WATCHDOG_ERROR);
   
    throw new passwordChangerApiExeption('[CONFLICT]' . t("Module(s) @allow allow @property and module(s) @deny deny @property this is causing a conflict.",
               array( '@allow' => trim($requirements['allow'], ','), '@property' => $property, '@deny' => trim($requirements['deny'], ','))) ); 
    
  }
  elseif (isset($requirements['require']) && isset($requirements['deny'])) {

    watchdog('[password_changer_api_complexity][_check_global_conflicts]', "Module(s) @require require @property and module(s) @deny deny @property this is causing a conflict.",
            array('@require' => trim($requirements['require'], ','), '@property' => $property, '@deny' => trim($requirements['deny'], ',')), WATCHDOG_ERROR);
 
    throw new passwordChangerApiExeption('[CONFLICT] ' . t("Module(s) @require require @property and module(s) @deny deny @property this is causing a conflict.",
                array('@require' => trim($requirements['require'], ','), '@property' => $property, '@deny' => trim($requirements['deny'], ','))) ); 
  } 
 }
 
 //If it reach this point then no conflicts detected.
 return TRUE;
 
}

/*
 * Check if there is more than one system with password complexity define.
 *
 * Helper function to check if there is more than one system with password complexity.
 *
 * @param array $register_module an with all the register modules and their properties.
 *
 * @return array $min_maximum value with the name of the module with the minimum
 *   maximum value.
 *
 * @param array $register_module store all register modules and their properties.
 * 
 */
function _password_changer_api_systems_with_complexity($register_modules) {
 //Initializing variable
 //How many modules have password complexity define
 $number_password_complexity = 0;
 //Loop through register modules to check if more than one has a register module
 foreach ( $register_modules AS $module_name => $properties ) {
  if (!empty($properties['#complexity'])) {
  $number_password_complexity++;
  }
  else {
   break;
  }
 }
 //Return number of modules that have password complexity
 return $number_password_complexity;
}

/*
 * Helper function that returns a list properties and modules that deny and require
 * the property.
 *
 * Helper function that returns a list properties (max_length, min_length, etc)
 * and modules that deny and require that property.
 *
 * @param array $register_module an with all the register modules and their rules.
 *
 * @return array $potential_conflicts array build dynamically with properties and modules
 * that deny or require the complexity rule.
 * 
 */

function _password_changer_api_get_potential_conflicts($register_module) {
 
 //Create conflict
 $potential_conflicts = array();
 
 //for each register module
 foreach ($register_module AS $module_name => $properties) {
  
  //Check if they have complexity define
   if (!empty($properties['#complexity'])) {
  
    //foreach complexity property 
    foreach ($properties['#complexity'] AS $property_name => $property_value ) {
     
     //if property is different the max_length or min_length
     //NOTE: Length conflicts are check in _password_changer_api_check_global_conflicts
     //the first if..
     if ($property_name != 'max_length' && $property_name != 'min_length') {
       //Module name out array
       $name = print_r($module_name, TRUE);
       
       //If property is set add module to the proper list, if
       //require put it in the require list, if deny put in the
       //deny list or if allow in the allow list.
       if (isset($potential_conflicts[$property_name])) {
         if ($property_value['value'] > 0) {
           @$potential_conflicts[$property_name]['require'] .= $name . ',';
         }
         elseif ($property_value['value'] < 0) {           
           @$potential_conflicts[$property_name]['deny'] .= $name . ',';
         }
         else {
           @$potential_conflicts[$property_name]['allow'] .= $name . ',';
         }
       }
       //If property is not set create the list and add module
       //to the proper list, if require in the require list
       //if deny in the deny and list if allow in the allow list
       else {
        if ($property_value['value'] > 0) {
         @$potential_conflicts[$property_name] = array('require' => $name . ',');
        }
        elseif ($property_value['value'] < 0) {
         @$potential_conflicts[$property_name] = array('deny' => $name . ',');
        }
        else {
         @$potential_conflicts[$property_name] = array('allow' => $name . ','); 
        }
       }  
     }
    }
  }  
 }

 //Return potential conflicts array
 return $potential_conflicts;
}

/*
 * Check password against the password complexity for each module install.
 *
 * @param string $password user new password.
 * 
*/


function password_changer_api_check_complexity($password) {
 
 _password_changer_api_check_property_requirement($password);
 
 //Call the module hook to change passwords
 module_invoke_all('password_changer_api_check_complexity');
  
}