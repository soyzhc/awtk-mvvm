const fs = require('fs')

class Utils {

  static genIncludes(name) {
    let result = `#include "tkc/mem.h"\n`;
    result += `#include "tkc/utils.h"\n`;
    result += `#include "mvvm/base/utils.h"\n`;
    result += `#include "${name}.h"\n`;

    return result;
  }

  static saveResult(name, includes, header, content) {
    let defaultInclude = Utils.genIncludes(name);

    if(includes) {
      defaultInclude += includes.map(iter => {
        return `#include "${iter}"`
      }).join('');
    }
    defaultInclude += '\n';

    fs.writeFileSync(`${name}.h`, "\ufeff" + header);
    fs.writeFileSync(`${name}.c`, "\ufeff" + defaultInclude + content);
    console.log(`output to ${name}.h and ${name}.c`);
  }

  static genPropDecls(json) {
    let propsDecl = '';
    if (json.props && json.props.length) {
      propsDecl = json.props.map((prop) => {
        if (prop.fake) {
          return '';
        } else {
          if(prop.type === "char*") {
            return `  str_t ${prop.name};`;
          } else {
            return `  ${prop.type} ${prop.name};`;
          }
        }
      }).join('\n');
    }

    if(propsDecl.length < 2) {
      propsDecl = "   uint32_t dummy;";
    }

    return propsDecl;
  }

  static genGetPropsDispatch(json) {
    const clsName = json.name;
    return json.props.map((prop, index) => {
      let str = '  ';
      const propName = prop.name;

      if (prop.private) {
        return str;
      }

      if (index === 0) {
        str += 'if (';
      } else {
        str += '} else if (';
      }

      str += `tk_str_eq("${propName}", name)) {\n`
      if (prop.getter || prop.fake) {
        str += '    ' + Utils.genToValue(clsName, prop.type, propName, true);
      } else {
        str += '    ' + Utils.genToValue(clsName, prop.type, propName, false);
      }
      return str;
    }).join('\n');
  }
  
  static genSetPropsDispatch(json) {
    const clsName = json.name;
    return json.props.map((prop, index) => {
      let str = '  ';
      const propName = prop.name;
      if (prop.private) {
        return str;
      }

      if (index === 0) {
        str += 'if (';
      } else {
        str += '} else if (';
      }
      str += `tk_str_eq("${propName}", name)) {\n`
      if (prop.setter || prop.fake) {
        str += `    ${clsName}_set_${propName}(${clsName}, v);`;
      } else {
        str += `    ${Utils.genAssignValue(clsName, prop.type, prop.name)};`;
      }
      return str;
    }).join('\n');
  }

  static genToValue(clsName, type, name, getter) {
    switch (type) {
      case 'int8_t':
      case 'int16_t':
      case 'int32_t':
      case 'int64_t':
      case 'uint8_t':
      case 'uint16_t':
      case 'uint32_t':
      case 'uint64_t':
      case 'bool_t':
      case 'float_t':
      case 'float':
      case 'double': {
        let typeName = type.replace(/_t$/, '');
        if(getter) {
          return `value_set_${typeName}(v, ${clsName}_get_${name}(${clsName}));`;
        } else {
          return `value_set_${typeName}(v, ${clsName}->${name});`;
        }
      }
      case 'char*': {
        if(getter) {
          return `value_set_str(v, ${clsName}_get_${name}(${clsName}));`;
        } else {
          return `value_set_str(v, ${clsName}->${name}.str);`;
        }
      }
      case 'void*': {
        if(getter) {
          return `value_set_pointer(v, ${clsName}_get_${name}(${clsName}));`;
        } else {
          return `value_set_pointer(v, ${clsName}->${name});`;
        }
      }
      default: {
        console.log(`not supported ${type} for ${name}`);
        process.exit(0);
      }
    }
  }

  static genFromValue(clsName, type, name) {
    switch (type) {
      case 'int8_t':
      case 'int16_t':
      case 'int32_t':
      case 'int64_t':
      case 'uint8_t':
      case 'uint16_t':
      case 'uint32_t':
      case 'uint64_t':
      case 'bool_t':
      case 'float_t':
      case 'float':
      case 'double': {
        let typeName = type.replace(/_t$/, '');
        return `value_${typeName}(v)`;
      }
      case 'char*': {
        return `str_from_value(&(${clsName}->${name}), v)`;
      }
      case 'void*': {
        return `value_pointer(v)`;
      }
      default: {
        console.log(`not supported ${type} for ${name}`);
        process.exit(0);
      }
    }
  }

  static genAssignValue(clsName, type, name) {
    if (type === 'char*') {
      return `${Utils.genFromValue(clsName, type, name)}`;
    } else {
      return `${clsName}->${name} =  ${Utils.genFromValue(clsName, type, name)}`;
    }
  }
  
  static genModelCommonFuncs(json) {
    const clsName = json.name;

    let defaultInit = '';
    let defulatDeinit = '';
    if(json.props) {
      defulatDeinit = json.props.map(iter => {
        if(!iter.fake && iter.type === 'char*') {
          return `  str_reset(&(${clsName}->${iter.name}));\n`;
        } else {
          return '';
        }
      }).join('');
      
      defaultInit = json.props.map(iter => {
        if(iter.fake) {
          return '';
        }

        if(iter.type === 'char*') {
          if(iter.init_value) {
            return `  ${iter.init_value};\n`;
          } else {
            return `  str_init(&(${clsName}->${iter.name}), 10);\n`;
          }
        } else {
          if(iter.init_value) {
            return `  ${clsName}->${iter.name} = ${iter.init_value};\n`;
          }
        }
      }).join('');
    }

    let init = json.init ? json.init : defaultInit;
    let deinit = json.deinit ? json.deinit : defulatDeinit;
    let cmp = json.cmp ? json.cmp : "/*TODO: */\n  return 0;"

    let cmpFunc = '';
    if(json.collection) {
    cmpFunc = 
`

int ${clsName}_cmp(${clsName}_t* a, ${clsName}_t* b) {
  return_value_if_fail(a != NULL && b != NULL, -1);
${cmp}
}

`
    }
    let result =
      `
/***************${clsName}***************/;

${clsName}_t* ${clsName}_create(void) {
  ${clsName}_t* ${clsName} = TKMEM_ZALLOC(${clsName}_t);
  return_value_if_fail(${clsName} != NULL, NULL);

${init}

  return ${clsName};
} 
${cmpFunc}
static ret_t ${clsName}_destroy(${clsName}_t* ${clsName}) {
  return_value_if_fail(${clsName} != NULL, RET_BAD_PARAMS);

${deinit}

  TKMEM_FREE(${clsName});

  return RET_OK;
}

`
    return result;
  }
  
  static genPropFuncs(json) {
    const clsName = json.name;
    const result = json.props.map((prop, index) => {
      let str = '';
      const propName = prop.name;

      if (prop.private) {
        return str;
      }

      if (prop.fake) {
        let setter = typeof (prop.setter) === 'string' ? prop.setter : 'return RET_OK;';
        let getter = typeof (prop.getter) === 'string' ? prop.getter : 'return 0;';
        str =
          `
static ${prop.type} ${clsName}_get_${propName}(${clsName}_t* ${clsName}) {
  ${getter}
}

static ret_t ${clsName}_set_${propName}(${clsName}_t* ${clsName}, const value_t* v) {
  ${setter}
}

`;
      } else {
        if (prop.getter) {
          let defaultGetter = `return ${clsName}->${propName}`;
          if(prop.type === "char*") {
            defaultGetter += ".str";
          }
           defaultGetter += ";";
          let getter = typeof (prop.getter) === 'string' ? prop.getter : defaultGetter;
          str =
            `
static ${prop.type} ${clsName}_get_${propName}(${clsName}_t* ${clsName}) {
  ${getter}
}

`;
        }

        if (prop.setter) {
          let defaultSetter =
            `${Utils.genAssignValue(clsName, prop.type, propName)};

  return RET_OK;`

          let setter = typeof (prop.setter) === 'string' ? prop.setter : defaultSetter;
          str +=
            `
static ret_t ${clsName}_set_${propName}(${clsName}_t* ${clsName}, const value_t* v) {
  ${setter}
}

`;
        }
      }
      return str;
    }).join("");

    return result;
  }
  
  static genCmdFuncs(json) {
    const clsName = json.name;
    const result = json.cmds.map((cmd, index) => {
      let str = '';
      const cmdName = cmd.name;
      if (!cmd.canExec || typeof cmd.canExec === 'string') {
        let canExec = (typeof cmd.canExec === 'string') ? cmd.canExec : 'return TRUE;';
        str =
          `static bool_t ${clsName}_can_exec_${cmdName}(${clsName}_t* ${clsName}, const char* args) {
  ${canExec}
}\n\n`;
      }

      let impl = cmd.impl || '';
      str +=
        `static ret_t ${clsName}_${cmdName}(${clsName}_t* ${clsName}, const char* args) {
  ${impl}
  return RET_OBJECT_CHANGED;
}\n\n`;
      return str;
    }).join('\n');

    return result;
  }

}

module.exports = Utils;
