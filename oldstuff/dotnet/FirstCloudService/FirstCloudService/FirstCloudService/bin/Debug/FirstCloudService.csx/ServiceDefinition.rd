<?xml version="1.0"?>
<serviceModel xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xsd="http://www.w3.org/2001/XMLSchema" name="FirstCloudService" generation="1" functional="0" release="0" Id="2b7f969b-efcf-4180-9506-e01fb4f76a2e" dslVersion="1.2.0.0" xmlns="http://schemas.microsoft.com/dsltools/RDSM">
  <groups>
    <group name="FirstCloudServiceGroup" generation="1" functional="0" release="0">
      <componentports>
        <inPort name="HttpIn" protocol="http">
          <inToChannel>
            <lBChannelMoniker name="/FirstCloudService/FirstCloudServiceGroup/FELoadBalancerHttpIn" />
          </inToChannel>
        </inPort>
      </componentports>
      <settings>
        <aCS name="WebRoleInstances" defaultValue="[1,1,1]">
          <maps>
            <mapMoniker name="/FirstCloudService/FirstCloudServiceGroup/MapWebRoleInstances" />
          </maps>
        </aCS>
      </settings>
      <channels>
        <lBChannel name="FELoadBalancerHttpIn">
          <toPorts>
            <inPortMoniker name="/FirstCloudService/FirstCloudServiceGroup/WebRole/HttpIn" />
          </toPorts>
        </lBChannel>
      </channels>
      <maps>
        <map name="MapWebRoleInstances" kind="Identity">
          <setting>
            <sCSPolicyIDMoniker name="/FirstCloudService/FirstCloudServiceGroup/WebRoleInstances" />
          </setting>
        </map>
      </maps>
      <components>
        <groupHascomponents>
          <role name="WebRole" generation="1" functional="0" release="0" software="C:\Users\bcherry\Documents\Personal\Software Devlopment\dotnet\FirstCloudService\FirstCloudService\FirstCloudService\obj\Debug\FirstCloudService_WebRole\" entryPoint="ucruntime" parameters="Microsoft.ServiceHosting.ServiceRuntime.Internal.WebRoleMain" hostingEnvironment="frontend">
            <componentports>
              <inPort name="HttpIn" protocol="http" />
            </componentports>
            <resourcereferences>
              <resourceReference name="EventStore" defaultAmount="[1000,1000,1000]" defaultSticky="false" kind="LogStore" />
            </resourcereferences>
            <eventstreams>
              <eventStream name="Microsoft.ServiceHosting.ServiceRuntime.RoleManager.Critical" kind="Default" severity="Critical" signature="Basic_string" />
              <eventStream name="Microsoft.ServiceHosting.ServiceRuntime.RoleManager.Error" kind="Default" severity="Error" signature="Basic_string" />
              <eventStream name="Critical" kind="Default" severity="Critical" signature="Basic_string" />
              <eventStream name="Error" kind="Default" severity="Error" signature="Basic_string" />
              <eventStream name="Warning" kind="OnDemand" severity="Warning" signature="Basic_string" />
              <eventStream name="Information" kind="OnDemand" severity="Info" signature="Basic_string" />
              <eventStream name="Verbose" kind="OnDemand" severity="Verbose" signature="Basic_string" />
            </eventstreams>
          </role>
          <sCSPolicy>
            <sCSPolicyIDMoniker name="/FirstCloudService/FirstCloudServiceGroup/WebRoleInstances" />
          </sCSPolicy>
        </groupHascomponents>
      </components>
      <sCSPolicy>
        <sCSPolicyID name="WebRoleInstances" defaultPolicy="[1,1,1]" />
      </sCSPolicy>
    </group>
  </groups>
  <implements>
    <implementation Id="cf23ecf7-ccf4-4d71-853e-4d2f4c5f8745" ref="Microsoft.RedDog.Contract\ServiceContract\FirstCloudServiceContract@ServiceDefinition">
      <interfacereferences>
        <interfaceReference Id="ee1b6767-3e70-4041-833b-29dcdecfac0c" ref="Microsoft.RedDog.Contract\Interface\HttpIn@ServiceDefinition">
          <inPort>
            <inPortMoniker name="/FirstCloudService/FirstCloudServiceGroup/HttpIn" />
          </inPort>
        </interfaceReference>
      </interfacereferences>
    </implementation>
  </implements>
</serviceModel>