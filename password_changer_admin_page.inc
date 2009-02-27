<?php
// $Id$

/**
 * @file
 * FHCHS Universal Password Changer
 *
 * Admin Page
 * 
 */
 
function password_changer_admin_page($form_state) {
  $form['Name'] = array(
    '#type' => 'fieldset',
    '#title' => t('Name'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['Name']['preferred'] = array(
    '#type' => 'textfield',
    '#title' => t('Preferred Name'),
    '#default_value' => $form_state['values']['preferred'],
    '#description' => "The name you would like us to refer to you by. For example, <em>Bob</em> if your first name is <em>Robert</em>.",
    '#maxlength' => 20,
  '#size' => 20,
  );
  $form['Name']['first'] = array(
    '#type' => 'textfield',
    '#title' => t('Legal First Name'),
    '#default_value' => $form_state['values']['first'],
    '#required' => TRUE,
    '#maxlength' => 30,
    '#size' => 30,
  );
  $form['Name']['middle'] = array(
    '#type' => 'textfield',
    '#title' => t('Middle Name'),
    '#default_value' => $form_state['values']['middle'],
    '#required' => FALSE,
    '#maxlength' => 15,
    '#size' => 15,
  );
  $form['Name']['last'] = array(
    '#type' => 'textfield',
    '#title' => t('Last Name'),
    '#default_value' => $form_state['values']['last'],
    '#required' => TRUE,
    '#maxlength' => 30,
    '#size' => 30,
  );
  $form['Name']['former_last'] = array(
    '#type' => 'textfield',
    '#description' => "Please list any previous last names. For example, Smith, Thompson.",
    '#title' => t('Former Last Name'),
    '#default_value' => $form_state['values']['former_last'],
    '#required' => FALSE,
    '#maxlength' => 30,
    '#size' => 30,
  );
  
  //Language is a two step question
  $form['Name']['language'] = array(
    '#type' => 'select',
    '#title' => t('Is English your native language?'),
    '#default_value' => $form_state['values']['language'],
    '#required' => TRUE,
        '#ahah' => array(
      'path' => ahah_helper_path(array('Name')),
      'wrapper' => 'name-information-wrapper',
      'method' => 'replace',
      'event' => 'change',
      'effect' => 'fade',
      ),
    '#options' => array(
      '' => '',
      '1' => t('Yes'),
      '0' => t('No')
      ),
  );

  $form['Name']['update_language'] = array(
    '#type' => 'submit',
    '#value' => t('language'),
    // Note that we can simply use the generic submit callback provided by the
    // ahah_helper module here!
    // All it does, is set $form_state['rebuild'] = TRUE.
    '#submit' => array('ahah_helper_generic_submit'),
    // We set the 'no-js' class, which means this submit button will be hidden
    // automatically by Drupal if JS is enabled.
    '#attributes' => array('class' => 'no-js'),
  );

// Contact Information
  $form['Contact_Information'] = array(
    '#type' => 'fieldset',
    '#title' => t('Contact Information'),
  '#prefix' => '<div id="contact-information-wrapper">', // This is our wrapper div.
    '#suffix' => '</div>',
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['Contact_Information']['countryofresidence'] = array(
    '#type' => 'select',
    '#title' => t('Country of Residence'),
    '#default_value' => ($form_state['values']['countryofresidence'] == ""?"UNITED STATES":$form_state['values']['countryofresidence']),
  '#ahah' => array(
     'path' => ahah_helper_path(array('Contact_Information')),
     'wrapper' => 'contact-information-wrapper',
     'method' => 'replace',
     'event' => 'change',
     'effect' => 'fade',
    ),
    '#options' => $countries,
    '#required' => TRUE,
  );
  $form['Contact_Information']['update_countryofresidence'] = array(
    '#type' => 'submit',
    '#value' => t('Country of Residence'),
    '#submit' => array('ahah_helper_generic_submit'),
    '#attributes' => array('class' => 'no-js'),
  );
  $form['Contact_Information']['streetaddress'] = array(
    '#type' => 'textfield',
    '#title' => t('Street Address'),
    '#default_value' => $form_state['values']['streetaddress'],
    '#required' => TRUE,
    '#maxlength' => 35,
    '#size' => 35,
  );
  $form['Contact_Information']['addresscity'] = array(
    '#type' => 'textfield',
    '#title' => t('City'),
    '#default_value' => $form_state['values']['addresscity'],
    '#required' => TRUE,
    '#maxlength' => 35,
    '#size' => 35,
  );
  $form['Contact_Information']['postalcode'] = array(
    '#type' => 'textfield',
    '#title' => t('Postal Code'),
    '#default_value' => $form_state['values']['postalcode'],
    '#required' => TRUE,
    '#maxlength' => 10,
    '#size' => 10,
  );
  $form['Contact_Information']['primaryphone'] = array(
    '#type' => 'textfield',
    '#title' => t('Primary Phone Number'),
    '#default_value' => $form_state['values']['primaryphone'],
    '#required' => TRUE,
    '#description' => "The first number we should dial when trying to reach you.",
    '#maxlength' => 15,
    '#size' => 15,
  );
  $form['Contact_Information']['cellphone'] = array(
    '#type' => 'textfield',
    '#title' => t('Cell Phone Number'),
    '#default_value' => $form_state['values']['cellphone'],
    '#required' => False,
    '#description' => "Mobile phone number we should dial when trying to reach you.",
    '#maxlength' => 15,
    '#size' => 15,
  );
  $form['Contact_Information']['alternatephone'] = array(
    '#type' => 'textfield',
    '#title' => t('Alternate Phone Number'),
    '#default_value' => $form_state['values']['alternatephone'],
    '#required' => FALSE,
    '#description' => "If we can't reach you at the first number, we'll try this one.",
    '#maxlength' => 15,
    '#size' => 15,
  );
  $form['Contact_Information']['email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email Address'),
    '#default_value' => ($form_state['values']['email'] == ""?$user->mail:$form_state['values']['email']),
    '#required' => FALSE,
    '#description' => "",
    '#maxlength' => 100,
    '#size' => 35,
  );
  
// Birthplace Information
  $form['Birthplace_Information'] = array(
    '#type' => 'fieldset',
    '#title' => t('Birthplace Information'),
    '#prefix' => '<div id="birthplace-information-wrapper">', // This is our wrapper div.
    '#suffix' => '</div>',
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['Birthplace_Information']['dateofbirth'] = array(
    '#type' => 'date_popup',
    '#title' => t('Date of Birth'),
    '#default_value' => $form_state['values']['dateofbirth'],
    '#date_format' => 'm/d/Y',
    '#date_year_range' => '-109:-12',
    '#required' => TRUE,
  );
  $form['Birthplace_Information']['countryofbirth'] = array(
    '#type' => 'select',
    '#title' => t('Country of Birth'),
    '#default_value' => ($form_state['values']['countryofbirth'] == ""?"UNITED STATES":$form_state['values']['countryofbirth']),
    '#options' => $countries,
   '#ahah' => array(
     'path' => ahah_helper_path(array('Birthplace_Information')),
     'wrapper' => 'birthplace-information-wrapper',
     'method' => 'replace',
     'event' => 'change',
     'effect' => 'fade',
    ),
    '#required' => TRUE,
  );
  $form['Birthplace_Information']['update_countryofbirth'] = array(
    '#type' => 'submit',
    '#value' => t('Country of Birth'),
    // Note that we can simply use the generic submit callback provided by the
    // ahah_helper module here!
    // All it does, is set $form_state['rebuild'] = TRUE.
    '#submit' => array('ahah_helper_generic_submit'),
    // We set the 'no-js' class, which means this submit button will be hidden
    // automatically by Drupal if JS is enabled.
    '#attributes' => array('class' => 'no-js'),
  );

    $form['Birthplace_Information']['update_countryofcitizenship'] = array(
      '#type' => 'submit',
      '#value' => t('Country of Citizenship'),
      // Note that we can simply use the generic submit callback provided by the
      // ahah_helper module here!
      // All it does, is set $form_state['rebuild'] = TRUE.
      '#submit' => array('ahah_helper_generic_submit'),
      // We set the 'no-js' class, which means this submit button will be hidden
      // automatically by Drupal if JS is enabled.
      '#attributes' => array('class' => 'no-js'),
    );
    $form['Birthplace_Information']['numberquestion'] = array(
      '#type' => 'select',
      '#title' => t('Please indicate the immigration or citizenship number you have'),
      '#default_value' => $form_state['values']['numberquestion'],
      '#options' => array(
        'Please Select' => t(''),
        // US Social Security Number makes SS Field appear and Number field go away
        'US Social Security Number' => t('US Social Security Number'),
        'Naturalized Citizen Number' => t('Naturalized Citizen Number'),
        'Permanent Resident Number' => t('Permanent Resident Number'),
        // Applied for and Online Only makes number field go away
        'Permanent Resident Number Applied For' => t('Permanent Resident Number Applied For'),
        'Taking Online Courses Only' => t('Taking Online Courses Only'),
        ),
        '#ahah' => array(
        'path' => ahah_helper_path(array('Birthplace_Information')),
          'wrapper' => 'birthplace-information-wrapper',
          'method' => 'replace',
          'event' => 'change',
          'effect' => 'fade',
        ),
      '#required' => TRUE,
    );
      $form['Birthplace_Information']['update_numberquestion'] = array(
      '#type' => 'submit',
      '#value' => t('Please indicate the immigration or citizenship number you have'),
      // Note that we can simply use the generic submit callback provided by the
      // ahah_helper module here!
      // All it does, is set $form_state['rebuild'] = TRUE.
      '#submit' => array('ahah_helper_generic_submit'),
      // We set the 'no-js' class, which means this submit button will be hidden
      // automatically by Drupal if JS is enabled.
      '#attributes' => array('class' => 'no-js'),
    );
  $form['Submit'] = array(
  '#type' => 'submit',
  '#value' => t('Save and Continue'),
  );
  return ($form);
}

function password_changer_admin_page_validate($form, &$form_state) {
  if ($form_state['values']['ssn'] != "" && !is_numeric($form_state['values']['ssn'])) {
    form_set_error('ssn', t("Please enter a valid Social Securit Number"));
  }
}
function password_changer_admin_page_submit($form, &$form_state) {
//   global $user;
//   date_default_timezone_set('UTC');
//   $name = ($form_state['values']['preferred'] == ""?$form_state['values']['first']:$form_state['values']['preferred']);
// //  $dob = $form_state['values']['dateofbirth']['year'] .'-'. $form_state['values']['dateofbirth']['month'] .'-'. $form_state['values']['dateofbirth']['day'] .' 00:00:00';
//   $dob = $form_state['values']['dateofbirth'];
//   //null values are interperted to 0 by the db_query sql injection fixing this makes it difficult to leave GED date NULL an arbitrary date (1/1/1800) was chosen by the developer to represent lack of GED NULL values
//   $GED_date = '1899-12-30 00:00:00';
//
//
//   if ($form_state['values']['language'] == 'I prefer not to share.') {
//     $form_state['values']['language'] = 'U';
//   }
//   if ($form_state['values']['ssn'] != NULL) {
//     $ssn = $form_state['values']['ssn'];
//   }
//   else {
//     $ssn = '0';
//   }
// //
// //   print_r($form);
// //   print_r($form_state);
// //   exit;
//
//   $a = "SELECT count(Student_StudentID) as count FROM {password_changer_Biographic} WHERE Student_StudentID = '%s'";
//   $a = db_fetch_array(db_query($a, $user->uid));
//
//   if ($a['count'] > 0) {
//     $biographic_sql = "UPDATE {password_changer_Biographic} SET Statistics_Native_English = '%s', Statistics_Native_Language = '%s', Student_Nickname = '%s', Student_Fname = '%s', Student_Mname = '%s', Student_Lname = '%s', Student_FormerLname = '%s', Student_DOB = '%s', Student_SSN = '%s', Birth_Country = '%s', Birth_City = '%s', Birth_State = '%s', Citizenship_Country = '%s', Citizenship_ID_Number = '%s', Citizenship_ID_Type = '%s', Citizenship_Visa_Status = '%s' WHERE  Student_StudentID = '%s'";
//     db_query($biographic_sql, $form_state['values']['language'], $form_state['values']['nativelanguage'], $form_state['values']['preferred'], $form_state['values']['first'], $form_state['values']['middle'], $form_state['values']['last'], $form_state['values']['former_last'], $dob , $ssn, $form_state['values']['countryofbirth'], $form_state['values']['cityofbirth'], $form_state['values']['stateofbirth'], $form_state['values']['countryofcitizenship'], $form_state['values']['number'], $form_state['values']['numberquestion'], $form_state['values']['visa'], $user->uid);
//   }
//   else {
//     $biographic_sql = "INSERT into {password_changer_Biographic} (Student_StudentID, Statistics_Native_English, Statistics_Native_Language, Student_Nickname, Student_Fname, Student_Mname, Student_Lname, Student_FormerLname, Student_DOB, Student_SSN, Birth_Country, Birth_City, Birth_State, Citizenship_Country, Citizenship_ID_Number, Citizenship_ID_Type, Citizenship_Visa_Status, HighSchool_Name, HighSchool_Country, HighSchool_State, HighSchool_City, HighSchool_Attendance, HighSchool_Sonis_ID, GED_Testing_Site, GED_Date_Of_Issue, Statistics_Gender, Statistics_Hispanic, Statistics_Ethnic_Origin, Statistics_Native_English, Statistics_Native_Language, Statistics_Marital_Status, Statistics_Religious_Affiliation, Statistics_Home_Church, Statistics_Home_Church_City, Statistics_Home_Church_State, Personal_Statement_Agreement)VALUES('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%d', '%s', '%s', '%s', '%s', '%d', '%s', '%d', '%s', '%s', '%s', '%s', '%s', '%s', '%d')";
//     db_query($biographic_sql, $user->uid, $form_state['values']['language'], $form_state['values']['nativelanguage'], $form_state['values']['preferred'], $form_state['values']['first'], $form_state['values']['middle'], $form_state['values']['last'], $form_state['values']['former_last'], $dob , $ssn, $form_state['values']['countryofbirth'], $form_state['values']['cityofbirth'], $form_state['values']['stateofbirth'], $form_state['values']['countryofcitizenship'], $form_state['values']['number'], $form_state['values']['numberquestion'], $form_state['values']['visa'], '', '', '', '', '', '', '', $GED_date, '', '', '', '', '', '', '', '', '', '', 0);
//
//     //Fire Application Started Trigger
//     //This will never be called due to preempt in load_defaults below.
//     module_invoke_all('password_changer_changed', 'start', $user);
//   }
//
//
//   $a = "SELECT count(Student_StudentID) as count FROM {password_changer_Address} WHERE Student_StudentID = '%s'";
//   $a = db_fetch_array(db_query($a, $user->uid));
//
//   if ($a['count'] > 0) {
//     $address_sql = "UPDATE {password_changer_Address} SET Country = '%s', State = '%s', City = '%s', Postal = '%s', Street = '%s', Primary_phone = '%s', Mobile_phone = '%s', Alternate_phone = '%s', Email = '%s', Contact_Name = '%s' WHERE Student_StudentID = '%s' and Type = '%s'";
//     db_query($address_sql, $form_state['values']['countryofresidence'], $form_state['values']['state'],   $form_state['values']['addresscity'], $form_state['values']['postalcode'], $form_state['values']['streetaddress'], $form_state['values']['primaryphone'], $form_state['values']['cellphone'], $form_state['values']['alternatephone'], $form_state['values']['email'], $form_state['values']['preferred'], $user->uid, 'Pref Add');
//   }
//   else {
//     $address_sql = "INSERT into {password_changer_Address} (Student_StudentID, Country, State, City, Postal, Street, Primary_phone, Mobile_phone, Alternate_phone, Email, Type, Contact_Name)VALUES('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s')";
//     db_query($address_sql, $user->uid, $form_state['values']['countryofresidence'], $form_state['values']['state'],   $form_state['values']['addresscity'], $form_state['values']['postalcode'], $form_state['values']['streetaddress'], $form_state['values']['primaryphone'], $form_state['values']['cellphone'], $form_state['values']['alternatephone'], $form_state['values']['email'], 'Pref Add', $form_state['values']['preferred']);
//   }
//
//   //Fire Application Updated Trigger
//   module_invoke_all('password_changer_changed', 'update', $user);
//
//   drupal_set_message(t('%name, your Personal Information section has been saved.', array('%name' => $name)));
}
