<?php
function c2t2_print($form, &$form_state) {
	drupal_add_css(drupal_get_path('module', 'c2t2').'/css/printcenter.css');
	$c2t2_print_center = variable_get('c2t2_print_center', array());
	$printer_op = array_keys($c2t2_print_center['click']);
	$printer = isset($form_state['values']['printer']) ? $form_state['values']['printer'] : 3;
	$printer_name = $printer_op[$printer];
	$color_op = array_keys($c2t2_print_center['click'][$printer_name]);
	$color = isset($form_state['values']['color']) ? $form_state['values']['color'] : 0;
	$format = isset($form_state['values']['format']) ? $form_state['values']['format'] : 'Single Sided';
	
	dpm($c2t2_print_center);
	
	$form['settings'] = array('#markup'=>'<div><a class="button" href="/printcenter/settings">Settings</a></div>');
	$form['description']  = array('#type' => 'textfield','#title' => t('Description'),'#default_value' => '', '#description' => 'Line item title.');
	$form['numPages']  = array('#type' => 'textfield','#title' => t('Pages'),'#default_value' => '', '#description' => 'Number of pages per set');
	$form['copies']  = array('#type' => 'textfield','#title' => t('Copies'),'#default_value' => '', '#description' => 'Number of copies -or- sets');
	
	$form['click']['printer'] = array(
		'#type' => 'select',
		'#title' => t('Printer'),
		'#options' => $printer_op,
		'#default_value' => $printer,
		'#ajax' => array(
		  'callback' => 'c2t2_printer_callback',
		  'wrapper' => 'color-div',
		  'method' => 'replace',
		  'effect' => 'fade',
		  'keypress' => TRUE,		  
		),
	);
	$form['click']['color'] = array(
		'#type' => 'radios',
		'#options' => $color_op,
		'#default_value' => $color,
		'#prefix' => '<div id="color-div">',
		'#suffix' => '</div>',
	);
	
	$form['format'] = array(
		'#type' => 'radios',
		'#default_value' => $format,
		'#options' => array('Single Sided' => t('Single Sided'), 'Double Sided' => t('Double Sided'), 'Envelope' => t('Envelope'), 'Large Format' => t('Large Format'), 'Carbonless' => t('Carbonless'), ),
		'#ajax' => array(
		  'callback' => 'c2t2_format_options_callback',
		  'wrapper' => 'format_options_div',
		  'method' => 'replace',
		  'effect' => 'none',
		  'keypress' => TRUE,
		),		
	);

  switch($format) {
	  case 'Single Sided':
	  case 'Double Sided':
	  c2t2_sided($format, $form, $form_state);
	  break;
	  case 'Envelope':
	 c2t2_envelope($format, $form);
	  break;
	  case 'Large Format':
	  c2t2_large_format($format, $form);
	  break;
	  case 'Carbonless':
	  c2t2_carbonless($format, $form);
	  break;
	  default:
	  $form['format_options'] = array(
    // The prefix/suffix provide the div that we're replacing, named by $form['format'] #ajax['wrapper'] above.
	'#title' => t('Select Options Above'),
    '#prefix' => '<div id="format_options_div">',
    '#suffix' => '</div>',
    '#type' => 'fieldset',
  );
  }
  $form['button'] = array(
    '#type' => 'button',
	'#value' => t('Enter'),
	'#ajax' => array(
	  'callback' => 'c2t2_total_callback',
	  'wrapper' => 't_div',
	  'method' => 'replace',
	  'effect' => 'none',
	  'keypress' => TRUE,
	),
  );
  $form_state['values']['me']['click_price'] = $c2t2_print_center['click'][$printers[isset($form_state['values']['printer']) ? $form_state['values']['printer'] : 3]][$color[$form_state['values']['color']]];
  $form['total'] = array(
    // The prefix/suffix provide the div that we're replacing, named by $form['format'] #ajax['wrapper'] above.
	//'#title' => t('Select Options Above'),
    '#prefix' => '<div id="t_div">',
    '#suffix' => '</div>',
    '#type' => 'fieldset',
  );
  return $form;
}
/********************************************/
/******** Ajax Return Functions *************/
/********************************************/

function c2t2_printer_callback($form, $form_state) {
	return $form['click']['color'];
}
function c2t2_format_options_callback($form, $form_state) {	
	return $form['format_options'];
}
function c2t2_paper_size_callback($form, $form_state) {
	return $form['format_options']['paper_size_options'];
}
function c2t2_paper_callback($form, $form_state) {
	return $form['format_options']['paper_size_options']['paper_options'];
}
function c2t2_total_callback(&$form, &$form_state) {
	foreach( $form_state['values'] as $key => $value) {
		if( !is_array($value) ) {
			$form['total']['value_' . $key] = array('#type' => 'item', '#title' => t($key),	'#markup' => $value, );
		}else {
			foreach( $value as $k => $v) {
				if( !is_array($v) ) {
					$form['total']['value_' . $key . "_" . $k] = array('#type' => 'item', '#title' => t($key . ' -> ' . $k),	'#markup' => $v, );
				}else {
					$form['total']['value_' . $key . "_" . $k] = array('#type' => 'item', '#title' => t($key . ' ' . $k),	'#markup' => 'array', );
				}
			}
		}
	}
	return $form['total'];
}
/********************************************/
/******** Form Builder Functions ************/
/********************************************/
function c2t2_sided($format, &$form, &$form_state) {
	
	$field_papers_size = field_info_field('field_papers_size');
	$form['format_options'] = array(
    // The prefix/suffix provide the div that we're replacing, named by $form['format'] #ajax['wrapper'] above.
	'#title' => t($format),
    '#prefix' => '<div id="format_options_div">',
    '#suffix' => '</div>',
    '#type' => 'fieldset',
  );
	
	$form['format_options']['Finishing'] = array(
		'#type' => 'checkboxes',
		'#title' => t('Finishing'),
		'#options' => array('Staple' => t('Stapled'), '3Hole' => t('3 Hole Punch'), 'Saddle' => t('Saddle Stitch Staple'), 'Comb' => t('GBC Binding (comb)'), 'Letter_Fold' => t('Letter Fold'), 'Z_Fold' => t('Z Fold'), 'Shrink' => t('Shrink Wrapping'), 'Spiral' => t('Spiral Binding'), 'Laminating' => t('Laminating'), '1Hole' => t('Hydraulic 1-hole Punching (Calendars)')),
	);
	
	$size = !empty($form_state['values']['paper_size']) ? $form_state['values']['paper_size'] : 0;
	$options = $field_papers_size['settings']['allowed_values'];
	$options = array_merge(array(0=>'Select Paper Size'), $options);

	$form['format_options']['paper_size'] = array(
		'#type' => 'select',
		'#title' => t('Paper Size'),
		'#options' => $options,
		'#default_value' => $size,
		'#ajax' => array(
		  'callback' => 'c2t2_paper_size_callback',
		  'wrapper' => 'paper_size_options',
		  'method' => 'replace',
		  'effect' => 'fade',
		  'keypress' => TRUE,		  
		),
	);


  $form['format_options']['paper_size_options'] = array(
    '#title' => t(($size) ? $size : 'Select Paper Size'),
    // The prefix/suffix provide the div that we're replacing, named by #ajax['wrapper'] above.
    '#prefix' => '<div id="paper_size_options">',
    '#suffix' => '</div>',
    '#type' => 'fieldset',
  );
 
 if($size) {
	//https://www.drupal.org/node/1343708
	$query = new EntityFieldQuery();
	
	$query->entityCondition('entity_type', 'node')
	->entityCondition('bundle', 'paper')
	->propertyCondition('status', NODE_PUBLISHED)
	->fieldCondition('field_papers_size', 'value', $size, '=');
	$query->addMetaData('account', user_load(0)); // Run the query as user 0.
	$result = $query->execute();
	
	if (isset($result['node'])) {
		$papers_nids = array_keys($result['node']);
		$papers = entity_load('node', $papers_nids);
	}
	$paper_options = array();
	$b = FALSE;
	foreach($papers as $key => $paper) {
		$paper_options[$key] = $paper->title;
		if(!$b)
		  $b = $key; 
	}
	$a = !empty($form_state['values']['paper']) ? $form_state['values']['paper'] : $b;
	$form['format_options']['paper_size_options']['paper'] = array(
		'#type' => 'select',
		'#title' => t('Paper'),
		'#options' => $paper_options,
		'#default_value' => $a,
		'#ajax' => array(
		  'callback' => 'c2t2_paper_callback',
		  'wrapper' => 'paper-div',
		  'method' => 'replace',
		  'effect' => 'fade',
		  'keypress' => TRUE,		  
		),
	);

  $form['format_options']['paper_size_options']['paper_options'] = array(
    '#prefix' => '<div id="paper-div">',
    '#suffix' => '</div>',
    '#type' => 'fieldset',
  );
  foreach($papers[$a]->field_papers_colors[LANGUAGE_NONE] as $key=>$color) {
		$colors[$key]=$color['value'];
	}
	$form['format_options']['paper_size_options']['paper_options']['color'] = array(
	'#type' => 'select',
	'#title'=> 'Paper Color',
	'#options' => $colors,
	);
	$form['format_options']['paper_size_options']['paper_options']['weight'] = array(
	'#type' => 'item',
	'#title'=> 'Weight',
	'#markup' => $papers[$a]->field_papers_weight[LANGUAGE_NONE][0]['value'] . ' GSM',
	);
	$form['format_options']['paper_size_options']['paper_options']['price'] = array(
	'#type' => 'item',
	'#title'=> 'Price',
	'#markup' => 'Non-Member: ' . $papers[$a]->field_papers_price[LANGUAGE_NONE][0]['value']/1000*(1+variable_get('c2t2_paper_markup', 10)/100) . ' | Member: ' . $papers[$a]->field_papers_price[LANGUAGE_NONE][0]['value']/1000*(1+variable_get('c2t2_paper_member_markup', 5)/100),
	);
	
	
	
  }else {
	  $form['paper']["msg"] = array(
      '#markup' => '<h2>Set Paper Size</h2>',
    );
  }
}

function c2t2_envelope($format, &$form) {
	$form['format_options'] = array(
    // The prefix/suffix provide the div that we're replacing, named by $form['format'] #ajax['wrapper'] above.
	'#title' => t($format),
    '#prefix' => '<div id="format_options_div">',
    '#suffix' => '</div>',
    '#type' => 'fieldset',
  );
  $form['format_options']['Finishing'] = array(
		'#type' => 'checkboxes',
		'#title' => t('Finishing'),
		'#options' => array('Shrink' => t('Shrink Wrapping')),
	);
  $form['format_options']['Envelope'] = array(
		'#type' => 'radios',
		'#title' => t('Envelope'),
		'#options' => array('Letterhead' => t('Letterhead Stationary'), 'Return' => t('Return Address'), 'Window' => t('Window'), 'Plain' => t('Plain'), ),
	);
}
function c2t2_large_format($format, &$form) {
	$form['format_options'] = array(
    // The prefix/suffix provide the div that we're replacing, named by $form['format'] #ajax['wrapper'] above.
	'#title' => t($format),
    '#prefix' => '<div id="format_options_div">',
    '#suffix' => '</div>',
    '#type' => 'fieldset',
  );
  $form['format_options']['Finishing'] = array(
		'#type' => 'checkboxes',
		'#title' => t('Finishing'),
		'#options' => array(9 => t('Laminating')),
	);
}
function c2t2_carbonless($format, &$form) {
	$form['format_options'] = array(
    // The prefix/suffix provide the div that we're replacing, named by $form['format'] #ajax['wrapper'] above.
	'#title' => t($format),
    '#prefix' => '<div id="format_options_div">',
    '#suffix' => '</div>',
    '#type' => 'fieldset',
  );
  $form['format_options']['Finishing'] = array(
		'#type' => 'checkboxes',
		'#title' => t('Finishing'),
		'#options' => array('Staple' => t('Stapled'), '3Hole' => t('3 Hole Punch'), 'Shrink' => t('Shrink Wrapping'), 'Spiral' => t('Spiral Binding'), 'NCR' => t('NCR Glue'), 'Non-NCR' => t('Non-NCR Glue'), ),
	);
  $form['format_options']['Carbonless Forms'] = array(
		'#type' => 'radios',
		'#title' => t('Carbonless Forms'),
		'#options' => array(2 => t('2 Part Forms'), 3 => t('3 Part Forms'), 4 => t('4 Part Forms'), 5 => t('5 Part Forms'), ),
	);
}
