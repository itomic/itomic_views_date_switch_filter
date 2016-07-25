<?php 

class itomic_views_date_switch_filter_handler_filter_date_select extends views_handler_filter {

  function admin_summary() { }

  function option_definition() {
    $options = parent::option_definition();
    $options['breakpoint'] = array('default' => '-30 days');
    $options['label_before'] = array('default' => 'Past');
    $options['label_after'] = array('default' => 'Current');
    $options['date_field'] = array('default' => 'node.created');
    return $options;
  }

  function options_form(&$form, &$form_state) {

    parent::options_form($form, $form_state);

    $form['breakpoint'] = array(
      '#type' => 'textfield',
      '#title' => t('Breakpoint'),
      '#default_value' => $this->options['breakpoint'],
      '#description' => t('This will be fed through strtotime so you can use -30 days, -20 minutes'),
    );

    $form['label_before'] = array(
      '#type' => 'textfield',
      '#title' => t('Label Before'),
      '#default_value' => $this->options['label_before'],
    );

    $form['label_after'] = array(
      '#type' => 'textfield',
      '#title' => t('Label After'),
      '#default_value' => $this->options['label_after'],
    );

    $fields = date_views_fields($this->base_table);
    $options = array();
    foreach ($fields['name'] as $name => $field) {
      $options[$name] = $field['label'];
    }

    $form['date_field'] = array(
      '#title' => t('Date field'),
      '#type' => 'select',
      '#options' => $options,
      '#default_value' => $this->options['date_field'],
      '#description' => t('Select date field to filter.'),
      '#required' => TRUE,
    );

  }

  /**
   * Options form subform for setting exposed filter options.
   */
  function value_form(&$form, &$form_state) {
    parent::value_form($form, $form_state);

    $options = array(
        'after' => $this->options['label_after'],
        'before' => $this->options['label_before'],
    );

    $form['value'][$this->field] = array(
      '#type' => 'select',
      '#options' => $options,
      '#default_value' => $this->value[$this->field],
      '#title' => t('When')
    );

  }




  /**
   * Alters Views query when filter is used.
   */
  function query() {

    // make sure base table is included in the query
    $this->ensure_my_table();
    // retrieve real filter name from view options
    // this requires 'real field' filter option to be set (see code above)
    $real_field_name = $this->real_field;

    // get the value of the submitted filter
    $value = $this->view->exposed_data[$real_field_name];

    $field_value = $this->view->exposed_data[$this->field];

    $breakpoint = strtotime($this->options['breakpoint']);

    if($this->value[0] == 'before'){
        $this->query->add_where('AND', $this->options['date_field'], $breakpoint, '<');
    } else {
        $this->query->add_where('AND', $this->options['date_field'], $breakpoint, '>=');
    }

  }
}