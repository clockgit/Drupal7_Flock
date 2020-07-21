<?php
function c2t2_print_settings() {
	$header = array(
    'title' => array('data' => t('Title'), 'field' => 'n.title'),
    'type' => array('data' => t('Type'), 'field' => 'n.type'),
    'author' => t('Author'),
    'status' => array('data' => t('Status'), 'field' => 'n.status'),
    'changed' => array('data' => t('Updated'), 'field' => 'n.changed', 'sort' => 'desc')
  );
  $options = array();
  
  $query = new EntityFieldQuery();
	
	$query->entityCondition('entity_type', 'node')
	->entityCondition('bundle', 'paper')
	->propertyCondition('status', NODE_PUBLISHED);
	$query->addMetaData('account', user_load(0)); // Run the query as user 0.
	$result = $query->execute();
	
	$papers_nids = array_keys($result['node']);
	$papers = entity_load('node', $papers_nids);
	
	dpm($papers, 'r');
	
	foreach($papers as $node) {
		$options[$node->nid] = array(
      'title' => array(
        'data' => array(
          '#type' => 'link',
          '#title' => $node->title,
          '#suffix' => ' ' . theme('mark', array('type' => node_mark($node->nid, $node->changed))),
        ),
      ),
      'type' => check_plain(node_type_get_name($node)),
      'author' => theme('username', array('account' => $node)),
      'status' => $node->status ? t('published') : t('not published'),
      'changed' => format_date($node->changed, 'short'),
    ); 
	}
	
	$form['nodes'] = array(
      '#type' => 'tableselect',
      '#header' => $header,
      '#options' => $options,
      '#empty' => t('No content available.'),
    );
	
	
	
	//
	
	
	
	drupal_add_css(drupal_get_path('module', 'c2t2').'/css/printcenter.css');
	$form['printcenter_link'] = array('#markup'=>'<div><a class="button" href="/printcenter">Quotes</a></div>',);
	$form['add_paper'] = array('#markup'=>'<div><a class="button" href="/node/add/papers?destination=printcenter/settings">Add Paper Type</a></div>',);
	$form['view_paper'] = array('#markup'=>'<div><a class="button" href="/printcenter/paper?destination=printcenter/settings">Paper List</a></div>',);
	$form['#tree'] = TRUE;	// Prevent flattening the form values
	
	
	$form['c2t2_print_center'] = array(
		'#title' => t('Print Center Vars'),
		'#type'  => 'fieldset',
		'#collapsible' => FALSE,
	);
	#variable_del('c2t2_print_center');
	$c2t2_print_center = variable_get('c2t2_print_center', array());
	
	/** markup **/
	$form['c2t2_print_center']['makrup'] = array('#type'=>'fieldset','#title'=>'Pricing Markup', '#collapsible' => TRUE, '#collapsed' => TRUE,);
	
	$form['c2t2_print_center']['makrup']['member'] = array('#type'=>'fieldset','#title'=>'Member', );
	$form['c2t2_print_center']['makrup']['member']['click_markup']  = array('#type' => 'textfield','#title' => t('% Click Markup'),'#default_value' => (isset($c2t2_print_center['makrup']['member']['click_markup'])) ? $c2t2_print_center['makrup']['member']['click_markup']  : 5,);
	$form['c2t2_print_center']['makrup']['member']['paper_markup']  = array('#type' => 'textfield','#title' => t('% Paper Markup'),'#default_value' => (isset($c2t2_print_center['makrup']['member']['paper_markup'])) ? $c2t2_print_center['makrup']['member']['paper_markup']  : 5,);
	$form['c2t2_print_center']['makrup']['member']['labor']  = array('#type' => 'textfield','#title' => t('Labor'),'#default_value' => (isset($c2t2_print_center['makrup']['member']['labor'])) ? $c2t2_print_center['makrup']['member']['labor']  : 15,);
	
	$form['c2t2_print_center']['makrup']['non_member'] = array('#type'=>'fieldset','#title'=>'Non-Member', );
	$form['c2t2_print_center']['makrup']['non_member']['click_markup']  = array('#type' => 'textfield','#title' => t('% Click Markup'),'#default_value' => (isset($c2t2_print_center['makrup']['non_member']['click_markup'])) ? $c2t2_print_center['makrup']['non_member']['click_markup']  : 10,);
	$form['c2t2_print_center']['makrup']['non_member']['paper_markup']  = array('#type' => 'textfield','#title' => t('% Paper Markup'),'#default_value' => (isset($c2t2_print_center['makrup']['non_member']['paper_markup'])) ? $c2t2_print_center['makrup']['non_member']['paper_markup']  : 10,);
	$form['c2t2_print_center']['makrup']['non_member']['labor']  = array('#type' => 'textfield','#title' => t('Labor'),'#default_value' => (isset($c2t2_print_center['makrup']['non_member']['labor'])) ? $c2t2_print_center['makrup']['non_member']['labor']  : 25,);
	
	/** Click **/
	$form['c2t2_print_center']['click'] = array('#type'=>'fieldset','#title'=>'Click', '#collapsible' => TRUE, '#collapsed' => TRUE,);
	$form['c2t2_print_center']['click']['harpo'] = array('#type'=>'fieldset','#title'=>'Harpo',);
	$form['c2t2_print_center']['click']['harpo']['color'] = array('#type' => 'textfield','#title' => t('Color'),'#default_value' => (isset($c2t2_print_center['click']['harpo']['color'])) ? $c2t2_print_center['click']['harpo']['color']  : 0.080,);
	$form['c2t2_print_center']['click']['harpo']['black'] = array('#type' => 'textfield','#title' => t('Black'),'#default_value' => (isset($c2t2_print_center['click']['harpo']['black'])) ? $c2t2_print_center['click']['harpo']['black']  : 0.0120,);
	
	$form['c2t2_print_center']['click']['denzel'] = array('#type'=>'fieldset','#title'=>'Denzel');
	$form['c2t2_print_center']['click']['denzel']['color'] = array('#type' => 'textfield','#title' => t('Color'),'#default_value' => (isset($c2t2_print_center['click']['denzel']['color'])) ? $c2t2_print_center['click']['denzel']['color']  : 0.080,);
	$form['c2t2_print_center']['click']['denzel']['black'] = array('#type' => 'textfield','#title' => t('Black'),'#default_value' => (isset($c2t2_print_center['click']['denzel']['black'])) ? $c2t2_print_center['click']['denzel']['black']  : 0.0120,);
	
	$form['c2t2_print_center']['click']['jack'] = array('#type'=>'fieldset','#title'=>'jack');
	$form['c2t2_print_center']['click']['jack']['color'] = array('#type' => 'textfield','#title' => t('Color'),'#default_value' => (isset($c2t2_print_center['click']['jack']['color'])) ? $c2t2_print_center['click']['jack']['color']  : 0.050,);
	$form['c2t2_print_center']['click']['jack']['black'] = array('#type' => 'textfield','#title' => t('Black'),'#default_value' => (isset($c2t2_print_center['click']['jack']['black'])) ? $c2t2_print_center['click']['jack']['black']  : 0.0080,);
	
	$form['c2t2_print_center']['click']['bertha'] = array('#type'=>'fieldset','#title'=>'Bertha');
	$form['c2t2_print_center']['click']['bertha']['color'] = array('#type' => 'textfield','#title' => t('Color'),'#default_value' => (isset($c2t2_print_center['click']['bertha']['color'])) ? $c2t2_print_center['click']['bertha']['color']  : 0.04750,);
	$form['c2t2_print_center']['click']['bertha']['black'] = array('#type' => 'textfield','#title' => t('Black'),'#default_value' => (isset($c2t2_print_center['click']['bertha']['black'])) ? $c2t2_print_center['click']['bertha']['black']  : 0.0060,);
	
	$form['c2t2_print_center']['click']['archie'] = array('#type'=>'fieldset','#title'=>'Archie');
	$form['c2t2_print_center']['click']['archie']['black'] = array('#type' => 'textfield','#title' => t('Black'),'#default_value' => (isset($c2t2_print_center['click']['archie']['black'])) ? $c2t2_print_center['click']['archie']['black']  : 0.0060,);
	
	$form['c2t2_print_center']['click']['jughead'] = array('#type'=>'fieldset','#title'=>'Jughead');
	$form['c2t2_print_center']['click']['jughead']['black'] = array('#type' => 'textfield','#title' => t('Black'),'#default_value' => (isset($c2t2_print_center['click']['jughead']['black'])) ? $c2t2_print_center['click']['jughead']['black']  : 0.0060,);
	/** Finishing **/
	$form['c2t2_print_center']['finishing'] = array('#type'=>'fieldset','#title'=>'Finishing', '#collapsible' => TRUE, '#collapsed' => TRUE,);
	
	$form['c2t2_print_center']['finishing']['member'] = array('#type'=>'fieldset','#title'=>'Member', );
	$form['c2t2_print_center']['finishing']['member']['laminating'] = array('#type' => 'textfield','#title' => t('laminating'),'#default_value' => (isset($c2t2_print_center['finishing']['member']['laminating'])) ? $c2t2_print_center['finishing']['member']['laminating']  : 0.5, '#description' => t('per sq foot'), );
	$form['c2t2_print_center']['finishing']['member']['shrink_wrap'] = array('#type' => 'textfield','#title' => t('shrink_wrap'),'#default_value' => (isset($c2t2_print_center['finishing']['member']['shrink_wrap'])) ? $c2t2_print_center['finishing']['member']['shrink_wrap']  : 0.15, '#description' => t('per piece'), );
	$form['c2t2_print_center']['finishing']['member']['stapling'] = array('#type' => 'textfield','#title' => t('stapling'),'#default_value' => (isset($c2t2_print_center['finishing']['member']['stapling'])) ? $c2t2_print_center['finishing']['member']['stapling']  : 0.01, '#description' => t('per staple'), );
	$form['c2t2_print_center']['finishing']['member']['page_insertion'] = array('#type' => 'textfield','#title' => t('page_insertion'),'#default_value' => (isset($c2t2_print_center['finishing']['member']['page_insertion'])) ? $c2t2_print_center['finishing']['member']['page_insertion']  : 0.005, '#description' => t('per piece'), );	
	$form['c2t2_print_center']['finishing']['member']['cutting']  = array('#type' => 'textfield','#title' => t('Cutting'),'#default_value' => (isset($c2t2_print_center['finishing']['member']['cutting'])) ? $c2t2_print_center['finishing']['member']['cutting']  : 1, '#description' => t('max 2.5" stack / cut'), );
	
	$form['c2t2_print_center']['finishing']['non_member'] = array('#type'=>'fieldset','#title'=>'Non-Member', );
	$form['c2t2_print_center']['finishing']['non_member']['laminating']  = array('#type' => 'textfield','#title' => t('laminating'),'#default_value' => (isset($c2t2_print_center['finishing']['non_member']['laminating'])) ? $c2t2_print_center['finishing']['non_member']['laminating']  : 1, '#description' => t('per sq foot'), );
	$form['c2t2_print_center']['finishing']['non_member']['shrink_wrap']  = array('#type' => 'textfield','#title' => t('shrink_wrap'),'#default_value' => (isset($c2t2_print_center['finishing']['non_member']['shrink_wrap'])) ? $c2t2_print_center['finishing']['non_member']['shrink_wrap']  : 0.3, '#description' => t('per piece'), );
	$form['c2t2_print_center']['finishing']['non_member']['stapling']  = array('#type' => 'textfield','#title' => t('stapling'),'#default_value' => (isset($c2t2_print_center['finishing']['non_member']['stapling'])) ? $c2t2_print_center['finishing']['non_member']['stapling']  : 0.02, '#description' => t('per staple'), );
	$form['c2t2_print_center']['finishing']['non_member']['page_insertion']  = array('#type' => 'textfield','#title' => t('page_insertion'),'#default_value' => (isset($c2t2_print_center['finishing']['non_member']['page_insertion'])) ? $c2t2_print_center['finishing']['non_member']['page_insertion']  : 0.01, '#description' => t('per piece'), );
	$form['c2t2_print_center']['finishing']['non_member']['cutting']  = array('#type' => 'textfield','#title' => t('Cutting'),'#default_value' => (isset($c2t2_print_center['finishing']['non_member']['cutting'])) ? $c2t2_print_center['finishing']['non_member']['cutting']  : 1, '#description' => t('max 2.5" stack / cut'), );
	/** Bindery **/
	$form['c2t2_print_center']['bindery'] = array('#type'=>'fieldset','#title'=>'Bindery', '#collapsible' => TRUE, '#collapsed' => TRUE,);
	
	$form['c2t2_print_center']['bindery']['member'] = array('#type'=>'fieldset','#title'=>'Member', );
	$form['c2t2_print_center']['bindery']['member']['fold_on']  = array('#type' => 'textfield','#title' => t('In Line Booklet Fold'),'#default_value' => (isset($c2t2_print_center['bindery']['member']['fold_on'])) ? $c2t2_print_center['bindery']['member']['fold_on']  : 1, '#description' => t('per page'), );
	$form['c2t2_print_center']['bindery']['member']['fold_off']  = array('#type' => 'textfield','#title' => t('Off Line Fold'),'#default_value' => (isset($c2t2_print_center['bindery']['member']['fold_off'])) ? $c2t2_print_center['bindery']['member']['fold_off']  : 1, '#description' => t('per page'), );
	$form['c2t2_print_center']['bindery']['member']['saddle_stitch']  = array('#type' => 'textfield','#title' => t('saddle_stitch'),'#default_value' => (isset($c2t2_print_center['bindery']['member']['saddle_stitch'])) ? $c2t2_print_center['bindery']['member']['saddle_stitch']  : 0.06, '#description' => t('per piece'), );
	$form['c2t2_print_center']['bindery']['member']['coil_sm']  = array('#type' => 'textfield','#title' => t('<1" Coil Binding (plastic)'),'#default_value' => (isset($c2t2_print_center['bindery']['member']['coil_sm'])) ? $c2t2_print_center['bindery']['member']['coil_sm']  : 0.06, '#description' => t('per piece'), );
	$form['c2t2_print_center']['bindery']['member']['coil_lg']  = array('#type' => 'textfield','#title' => t('>1" Coil Binding (plastic)'),'#default_value' => (isset($c2t2_print_center['bindery']['member']['coil_lg'])) ? $c2t2_print_center['bindery']['member']['coil_lg']  : 0.06, '#description' => t('per piece'), );
	$form['c2t2_print_center']['bindery']['member']['comb_sm']  = array('#type' => 'textfield','#title' => t('<1" Comb Binding (plastic)'),'#default_value' => (isset($c2t2_print_center['bindery']['member']['comb_sm'])) ? $c2t2_print_center['bindery']['member']['comb_sm']  : 0.06, '#description' => t('per piece'), );
	$form['c2t2_print_center']['bindery']['member']['comb_lg']  = array('#type' => 'textfield','#title' => t('<1" Comb Binding (plastic)'),'#default_value' => (isset($c2t2_print_center['bindery']['member']['comb_lg'])) ? $c2t2_print_center['bindery']['member']['comb_lg']  : 0.06, '#description' => t('per piece'), );
	$form['c2t2_print_center']['bindery']['member']['wire_sm']  = array('#type' => 'textfield','#title' => t('<9/16" Wire Binding'),'#default_value' => (isset($c2t2_print_center['bindery']['member']['wire_sm'])) ? $c2t2_print_center['bindery']['member']['wire_sm']  : 0.06, '#description' => t('per piece'), );
	$form['c2t2_print_center']['bindery']['member']['wire_lg']  = array('#type' => 'textfield','#title' => t('<9/16" Wite Binding'),'#default_value' => (isset($c2t2_print_center['bindery']['member']['wire_lg'])) ? $c2t2_print_center['bindery']['member']['wire_lg']  : 0.06, '#description' => t('per piece'), );
	$form['c2t2_print_center']['bindery']['member']['3_hole']  = array('#type' => 'textfield','#title' => t('Three Hole Punch'),'#default_value' => (isset($c2t2_print_center['bindery']['member']['3_hole'])) ? $c2t2_print_center['bindery']['member']['3_hole']  : 0.06, '#description' => t('in line / sheeet'), );
	$form['c2t2_print_center']['bindery']['member']['padding']  = array('#type' => 'textfield','#title' => t('Padding'),'#default_value' => (isset($c2t2_print_center['bindery']['member']['padding'])) ? $c2t2_print_center['bindery']['member']['padding']  : 0.06, '#description' => t('per sheeet'), );
	
	$form['c2t2_print_center']['bindery']['non_member'] = array('#type'=>'fieldset','#title'=>'Non-Member', );
	$form['c2t2_print_center']['bindery']['non_member']['fold_on']  = array('#type' => 'textfield','#title' => t('In Line Booklet Fold'),'#default_value' => (isset($c2t2_print_center['bindery']['non_member']['fold_on'])) ? $c2t2_print_center['bindery']['non_member']['fold_on']  : 1, '#description' => t('per page'), );
	$form['c2t2_print_center']['bindery']['non_member']['fold_off']  = array('#type' => 'textfield','#title' => t('Off Line Fold'),'#default_value' => (isset($c2t2_print_center['bindery']['non_member']['fold_off'])) ? $c2t2_print_center['bindery']['non_member']['fold_off']  : 1, '#description' => t('per page'), );
	$form['c2t2_print_center']['bindery']['non_member']['saddle_stitch']  = array('#type' => 'textfield','#title' => t('saddle_stitch'),'#default_value' => (isset($c2t2_print_center['bindery']['non_member']['saddle_stitch'])) ? $c2t2_print_center['bindery']['non_member']['saddle_stitch']  : 0.06, '#description' => t('per piece'), );
	$form['c2t2_print_center']['bindery']['non_member']['coil_sm']  = array('#type' => 'textfield','#title' => t('<1" Coil Binding (plastic)'),'#default_value' => (isset($c2t2_print_center['bindery']['non_member']['coil_sm'])) ? $c2t2_print_center['bindery']['non_member']['coil_sm']  : 0.06, '#description' => t('per piece'), );
	$form['c2t2_print_center']['bindery']['non_member']['coil_lg']  = array('#type' => 'textfield','#title' => t('>1" Coil Binding (plastic)'),'#default_value' => (isset($c2t2_print_center['bindery']['non_member']['coil_lg'])) ? $c2t2_print_center['bindery']['non_member']['coil_lg']  : 0.06, '#description' => t('per piece'), );
	$form['c2t2_print_center']['bindery']['non_member']['comb_sm']  = array('#type' => 'textfield','#title' => t('<1" Comb Binding (plastic)'),'#default_value' => (isset($c2t2_print_center['bindery']['non_member']['comb_sm'])) ? $c2t2_print_center['bindery']['non_member']['comb_sm']  : 0.06, '#description' => t('per piece'), );
	$form['c2t2_print_center']['bindery']['non_member']['comb_lg']  = array('#type' => 'textfield','#title' => t('<1" Comb Binding (plastic)'),'#default_value' => (isset($c2t2_print_center['bindery']['non_member']['comb_lg'])) ? $c2t2_print_center['bindery']['non_member']['comb_lg']  : 0.06, '#description' => t('per piece'), );
	$form['c2t2_print_center']['bindery']['non_member']['wire_sm']  = array('#type' => 'textfield','#title' => t('<9/16" Wire Binding'),'#default_value' => (isset($c2t2_print_center['bindery']['non_member']['wire_sm'])) ? $c2t2_print_center['bindery']['non_member']['wire_sm']  : 0.06, '#description' => t('per piece'), );
	$form['c2t2_print_center']['bindery']['non_member']['wire_lg']  = array('#type' => 'textfield','#title' => t('<9/16" Wite Binding'),'#default_value' => (isset($c2t2_print_center['bindery']['non_member']['wire_lg'])) ? $c2t2_print_center['bindery']['non_member']['wire_lg']  : 0.06, '#description' => t('per piece'), );
	$form['c2t2_print_center']['bindery']['non_member']['3_hole']  = array('#type' => 'textfield','#title' => t('Three Hole Punch'),'#default_value' => (isset($c2t2_print_center['bindery']['non_member']['3_hole'])) ? $c2t2_print_center['bindery']['non_member']['3_hole']  : 0.06, '#description' => t('in line / sheeet'), );
	$form['c2t2_print_center']['bindery']['non_member']['padding']  = array('#type' => 'textfield','#title' => t('Padding'),'#default_value' => (isset($c2t2_print_center['bindery']['non_member']['padding'])) ? $c2t2_print_center['bindery']['non_member']['padding']  : 0.06, '#description' => t('per sheeet'), );
	
	
	return system_settings_form($form);
}